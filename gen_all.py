import os


def generate_include_file(directory, output_file):
    with open(output_file, "w") as outfile:
        outfile.write('#include "noya/head.hpp" \n\n')
        for root, _, files in os.walk(directory):
            for file in files:
                if (
                    file.endswith(".hpp")
                    and file != "all.hpp"
                    and file != "debug.hpp"
                    and file != "head.hpp"
                ):
                    relative_path = os.path.relpath(os.path.join(root, file), directory)
                    include_statement = f'#include "{relative_path}"\n'
                    outfile.write(include_statement)
    print(f"Generated {output_file} with all .hpp includes.")


directory = "."
output_file = "noya/all.hpp"
generate_include_file(directory, output_file)
