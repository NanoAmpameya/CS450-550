import sys
import random
import argparse

def main():
    # Set up the command-line argument parser
    parser = argparse.ArgumentParser(description="Convert an OBJ file to unrolled C++ arrays.")
    parser.add_argument("input_file", help="The name of the OBJ file to process (e.g., teapot_low.obj)")
    parser.add_argument("--color-mode", choices=['vertex', 'face', 'solid'], default='vertex',
                        help="Generate colors per 'vertex' (default), per 'face', or a 'solid' color.")
    parser.add_argument("--color-rgb", type=float, nargs=3, metavar=('R', 'G', 'B'), default=[1.0, 1.0, 1.0],
                        help="The RGB color values (0.0 to 1.0) to use when --color-mode is 'solid'. Default is white (1.0 1.0 1.0).")
    args = parser.parse_args()

    # Initialize storage
    vertices = []
    faces = []

    # 1. Parse the OBJ file
    try:
        with open(args.input_file, "r") as f:
            for line in f:
                if line.startswith("v "):
                    parts = line.split()
                    vertices.append((float(parts[1]), float(parts[2]), float(parts[3])))
                elif line.startswith("f "):
                    # Extract the 1-based vertex index from formats like "2/2/2"
                    parts = line.split()
                    faces.append([int(p.split('/')[0]) for p in parts[1:4]])
    except FileNotFoundError:
        print(f"Error: The file '{args.input_file}' was not found.")
        sys.exit(1)

    # 2. Generate the unrolled arrays
    unrolled_vertices = []
    unrolled_colors = []

    # Pre-calculate the solid color to ensure consistency
    if args.color_mode == 'solid':
        solid_color = [round(args.color_rgb[0], 3), round(args.color_rgb[1], 3), round(args.color_rgb[2], 3)]

    for face in faces:
        # If in 'face' mode, generate one random color for the entire triangle
        if args.color_mode == 'face':
            face_color = [round(random.random(), 3), round(random.random(), 3), round(random.random(), 3)]
            
        for vertex_idx in face:
            # OBJ is 1-indexed, Python is 0-indexed
            v = vertices[vertex_idx - 1]
            unrolled_vertices.extend(v)
            
            # Apply color based on the selected mode
            if args.color_mode == 'vertex':
                unrolled_colors.extend([round(random.random(), 3), round(random.random(), 3), round(random.random(), 3)])
            elif args.color_mode == 'face':
                unrolled_colors.extend(face_color)
            elif args.color_mode == 'solid':
                unrolled_colors.extend(solid_color)

    # Calculate total vertex count
    total_vertices = len(faces) * 3

    # 3. Output directly to a .txt file
    output_filename = "arrays.txt"
    with open(output_filename, "w") as f:
        # Add a helpful comment at the top for the C++ developer
        f.write(f"// Total unrolled vertices: {total_vertices}\n")
        f.write(f"// Use: glDrawArrays(GL_TRIANGLES, 0, {total_vertices});\n\n")
        
        f.write("float vertices[] = {\n    ")
        f.write(", ".join([f"{val}f" for val in unrolled_vertices]))
        f.write("\n};\n\n")

        f.write("float colors[] = {\n    ")
        f.write(", ".join([f"{val}f" for val in unrolled_colors]))
        f.write("\n};\n")

    # Console feedback
    print(f"Successfully generated {output_filename} from {args.input_file}!")
    print(f"Color mode : {args.color_mode.capitalize()}")
    if args.color_mode == 'solid':
        print(f"Solid color: RGB({solid_color[0]}, {solid_color[1]}, {solid_color[2]})")
    print(f"Draw count : {total_vertices} vertices")

if __name__ == "__main__":
    main()