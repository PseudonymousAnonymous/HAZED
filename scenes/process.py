import os

def extract_vertex_positions(stl_file_path, output_file_path):
    with open(stl_file_path, 'r') as stl_file:
        lines = stl_file.readlines()

    vertex_positions = []
    for line in lines:
        # Check if the line contains the vertex position information (lines starting with "vertex").
        if line.strip().lower().startswith("vertex"):
            # Extract the three coordinates (x, y, z) from the line.
            coordinates = line.strip().lower().replace("vertex", "").split()
            x, y, z = map(float, coordinates)
            vertex_positions.append((x, y, z))

    # Write the vertex positions to the output text file.
    with open(output_file_path, 'w') as output_file:
        for x, y, z in vertex_positions:
            output_file.write(f"{x} {y} {z}\n")

def process_all_stl_files(input_dir, output_dir):
    # Create the output directory if it doesn't exist.
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    index = 1
    # Process each STL file in the input directory.
    for filename in os.listdir(input_dir):
        if filename.endswith(".stl"):
            stl_file_path = os.path.join(input_dir, filename)
            output_file_path = os.path.join(output_dir, f"{index}.ob")
            extract_vertex_positions(stl_file_path, output_file_path)
            index += 1

if __name__ == "__main__":
    input_dir = r"C:\Users\PseudonymousGarbage\Desktop\HAZEDLocal\scenes\data"
    output_dir = r"C:\Users\PseudonymousGarbage\Desktop\HAZEDLocal\scenes\data"
    process_all_stl_files(input_dir, output_dir)