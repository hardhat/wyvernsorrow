import json
import os
import re
from http.server import HTTPServer, SimpleHTTPRequestHandler
import urllib.parse

# Paths relative to this script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BASE_DIR = os.path.dirname(os.path.dirname(SCRIPT_DIR))
MAP_FILE_PATH = os.path.join(BASE_DIR, 'src', 'map.c')
IMG_PATH = os.path.join(BASE_DIR, 'img')

MAP_WIDTH = 20
MAP_HEIGHT = 15

# Extracted from main.h
TILES = [
    "TILE_OVERMAP_GRASS",
    "TILE_OVERMAP_FOREST",
    "TILE_OVERMAP_FOOTHILLS",
    "TILE_OVERMAP_MOUNTAIN",
    "TILE_OVERMAP_CITY",
    "TILE_OVERMAP_WATER",
    "TILE_OVERMAP_ROUGH_WATER",
    "TILE_BRICK_WALL",
    "TILE_DECAYED_WALL",
    "TILE_BRIGHT_WALL",
    "TILE_STATUE",
    "TILE_WALL_OPENING",
    "TILE_WALL_TAPESTRY",
    "TILE_ROOF",
    "TILE_CHAIR",
    "TILE_TABLE",
    "TILE_CANDLE",
    "TILE_STONE_FLOOR",
    "TILE_PIT",
    "TILE_UNUSED_1",
    "TILE_UNUSED_2",
    "TILE_WOOD_WALL",
    "TILE_WOOD_CABINET",
    "TILE_WOOD_DOOR",
    "TILE_WOOD_FLOOR",
    "TILE_LIGHT_WOOD_FLOOR",
    "TILE_UNUSED_3",
    "TILE_UNUSED_4"
]

def load_maps():
    """Reads src/map.c and parses the C arrays into Python dicts of strings."""
    try:
        with open(MAP_FILE_PATH, 'r') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Error: Could not find {MAP_FILE_PATH}")
        return {}

    maps = {}
    
    # Regex to find map definitions like: const uint8_t town_map[MAP_HEIGHT][MAP_WIDTH] = { ... };
    map_pattern = re.compile(r'const\s+uint8_t\s+([a-zA-Z0-9_]+)\[MAP_HEIGHT\]\[MAP_WIDTH\]\s*=\s*\{(.*?)\};', re.MULTILINE | re.DOTALL)
    
    for match in map_pattern.finditer(content):
        map_name = match.group(1)
        map_data_str = match.group(2)
        
        # Clean up comments and whitespace
        map_data_str = re.sub(r'//.*', '', map_data_str)
        # Extract individual rows
        row_pattern = re.compile(r'\{([^}]+)\}')
        
        rows = []
        for row_match in row_pattern.finditer(map_data_str):
            row_str = row_match.group(1)
            # Split by comma and strip whitespace
            tiles = [tile.strip() for tile in row_str.split(',') if tile.strip()]
            if tiles:
                rows.append(tiles)
                
        if len(rows) > 0:
            maps[map_name] = rows
            
    return maps

def save_maps(maps_data):
    """Takes the edited maps back and writes them safely into src/map.c"""
    try:
        with open(MAP_FILE_PATH, 'r') as f:
            content = f.read()
            
        new_content = content
            
        for map_name, map_grid in maps_data.items():
            # Build the new C array string
            c_array_lines = []
            for row in map_grid:
                c_array_lines.append("    {" + ", ".join(row) + "},")
            
            c_array_str = "\n".join(c_array_lines) + "\n"
            
            # Regex to replace just the contents of this specific map
            replace_pattern = re.compile(rf'(const\s+uint8_t\s+{map_name}\[MAP_HEIGHT\]\[MAP_WIDTH\]\s*=\s*\{{)\s*(.*?)\s*(\}};)', re.MULTILINE | re.DOTALL)
            
            new_content = replace_pattern.sub(rf'\1\n{c_array_str}\2', new_content)
            
        with open(MAP_FILE_PATH, 'w') as f:
            f.write(new_content)
            
        return True
    except Exception as e:
        print(f"Error saving maps: {e}")
        return False

class MapEditorHandler(SimpleHTTPRequestHandler):
    def do_GET(self):
        # Serve the app at root
        if self.path == '/' or self.path == '/index.html':
            try:
                with open(os.path.join(SCRIPT_DIR, 'index.html'), 'rb') as f:
                    self.send_response(200)
                    self.send_header('Content-Type', 'text/html')
                    self.end_headers()
                    self.wfile.write(f.read())
                    return
            except FileNotFoundError:
                self.send_error(404, 'File Not Found: index.html')
                return
        
        # Serve the terrain gif directly from img dir
        if self.path == '/img/terrain.gif':
            try:
                with open(os.path.join(IMG_PATH, 'terrain.gif'), 'rb') as f:
                    self.send_response(200)
                    self.send_header('Content-Type', 'image/gif')
                    self.end_headers()
                    self.wfile.write(f.read())
                    return
            except FileNotFoundError:
                self.send_error(404, 'File Not Found: terrain.gif')
                return
                
        # API: Get map data
        if self.path == '/api/maps':
            maps_data = load_maps()
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.end_headers()
            self.wfile.write(json.dumps({
                "maps": maps_data,
                "tiles": TILES,
                "width": MAP_WIDTH,
                "height": MAP_HEIGHT
            }).encode())
            return
            
        # Fallback to serving normal static files from current directory
        return SimpleHTTPRequestHandler.do_GET(self)
        
    def do_POST(self):
        # API: Save map data
        if self.path == '/api/maps':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            
            try:
                data = json.loads(post_data.decode('utf-8'))
                success = save_maps(data.get('maps', {}))
                
                if success:
                    self.send_response(200)
                    self.send_header('Content-type', 'application/json')
                    self.end_headers()
                    self.wfile.write(json.dumps({"status": "success"}).encode())
                else:
                    self.send_response(500)
                    self.end_headers()
            except json.JSONDecodeError:
                self.send_response(400)
                self.end_headers()
            return

def run_server(port=8080):
    server_address = ('', port)
    httpd = HTTPServer(server_address, MapEditorHandler)
    print(f"Starting map editor backend on http://localhost:{port}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nStopping server.")
        httpd.server_close()

if __name__ == '__main__':
    run_server()
