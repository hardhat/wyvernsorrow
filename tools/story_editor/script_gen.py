import json
import os

# Opcode Mapping
OPS = {
    "END": 0,
    "SAY": 1,
    "SET_FLAG": 2,
    "CLEAR_FLAG": 3,
    "MOVE": 4,
    "SET_STATE": 5,
    "GOTO": 6,
    "IF_FLAG_GOTO": 7,
}

# Flag Mapping (from world.h)
FLAGS = {
    "WFLAG_VISIBLE": 0x0001,
    "WFLAG_INTERACTABLE": 0x0002,
    "WFLAG_DEFEATED": 0x0004,
    "WFLAG_RESCUED": 0x0008,
    "WFLAG_CAPTIVE": 0x0010,
    "WFLAG_TALKED": 0x0020,
}

# Object Mapping (matching world.h)
OBJS = {
    "WOBJ_PLAYER": 32,
    "WNPC_SWORDSMAN": 33,
    "WNPC_MAGE": 34,
    "WNPC_WYVERN": 35,
    "WNPC_LITTLE_GIRL": 36,
    "WNPC_MOTHER": 37,
    "WNPC_FATHER": 38,
    "WNPC_CARPENTER": 39,
    "WNPC_OLD_MAN": 40,
    "WNPC_KING": 41,
    "WNPC_MERCHANT": 42,
    "WENT_GOBLIN": 64,
    "WITEM_FISH": 80,
    "WBOSS_OGRE": 112,
    "WBOSS_LAND_DRAGON": 113,
    "WBOSS_WIND_DRAGON": 114,
}

def generate_header(json_path, header_path):
    with open(json_path, 'r') as f:
        data = json.load(f)

    with open(header_path, 'w') as f:
        f.write("#ifndef SCRIPT_H\n#define SCRIPT_H\n\n")
        f.write("#include <stdint.h>\n\n")

        # Strings
        f.write("// ---- Strings ----\n")
        for i, s in enumerate(data['strings']):
            # Escape newlines and quotes
            escaped = s.replace('"', '\\"').replace('\n', '\\n')
            f.write(f"static const char s_str_{i}[] = \"{escaped}\";\n")
        
        f.write("\nstatic const char * const story_strings[] = {\n")
        for i in range(len(data['strings'])):
            f.write(f"    s_str_{i},\n")
        f.write("};\n\n")

        # Scripts
        f.write("// ---- Scripts ----\n")
        script_ptrs = {}
        for name, script in data['scripts'].items():
            bytes_list = []
            for cmd in script:
                op_name = cmd[0]
                bytes_list.append(str(OPS[op_name]))
                
                if op_name == "SAY":
                    bytes_list.append(str(cmd[1]))
                elif op_name in ["SET_FLAG", "CLEAR_FLAG"]:
                    obj = OBJS[cmd[1]]
                    flag = FLAGS[cmd[2]]
                    bytes_list.append(str(obj))
                    bytes_list.append(f"0x{flag & 0xFF:02X}")
                    bytes_list.append(f"0x{(flag >> 8) & 0xFF:02X}")
                elif op_name == "MOVE":
                    bytes_list.append(str(OBJS[cmd[1]]))
                    bytes_list.append(str(OBJS[cmd[2]]))
                elif op_name == "SET_STATE":
                    bytes_list.append(str(OBJS[cmd[1]]))
                    bytes_list.append(str(cmd[2]))
                elif op_name == "GOTO":
                    bytes_list.append(str(cmd[1]))
                elif op_name == "IF_FLAG_GOTO":
                    obj = OBJS[cmd[1]]
                    flag = FLAGS[cmd[2]]
                    bytes_list.append(str(obj))
                    bytes_list.append(f"0x{flag & 0xFF:02X}")
                    bytes_list.append(f"0x{(flag >> 8) & 0xFF:02X}")
                    bytes_list.append(str(cmd[3]))

            f.write(f"static const uint8_t script_{name}[] = {{ {', '.join(bytes_list)} }};\n")
            script_ptrs[name] = f"script_{name}"

        # Scene Mapping
        f.write("\n// ---- Scene Mapping ----\n")
        # For simplicity, we'll create a single lookup table or function
        # But for now let's just expose the scripts.
        
        f.write("#endif // SCRIPT_H\n")

if __name__ == "__main__":
    base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
    json_path = os.path.join(base_dir, "tools", "story_editor", "script.json")
    header_path = os.path.join(base_dir, "src", "script.h")
    generate_header(json_path, header_path)
    print(f"Generated {header_path}")
