import os
import subprocess
import argparse


def execute(command):
    print(' '.join(command))
    process = subprocess.run(command)
    if process.returncode is not 0:
        raise RuntimeError(f'exited with status code {process.returncode}')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Simple build command')
    parser.add_argument('--skip-tests', dest='skip_tests', action='store_true', default=False)
    args = parser.parse_args()

    files_to_format = []
    for path, _, files in os.walk('source'):
        source_files = [os.path.join(path, f) for f in files if f.endswith('.cpp') or f.endswith('.hpp')]
        files_to_format.extend(source_files)

    execute(['clang-format', '-i', '-style=file'] + files_to_format)

    if not args.skip_tests:
        execute(['scan-build', '-o', 'target/clang-analyzer', 'g++', '-std=c++1z', '-O3', '-o', 'target/eyecandy-test.exe',
                 'source/dansandu/eyecandy/math/interpolation.t.cpp',
                 'source/dansandu/eyecandy/math/matrix.t.cpp',
                 'source/dansandu/eyecandy/math/vector.t.cpp',
                 'source/dansandu/eyecandy/math/transformation.t.cpp',
                 'source/dansandu/eyecandy/raster/image.t.cpp',
                 'source/dansandu/eyecandy/raster/solid_rasterizer.t.cpp',
                 'source/dansandu/eyecandy/raster/wireframe_rasterizer.t.cpp',
                 'source/dansandu/eyecandy/raster/line_tracer.t.cpp',
                 'source/dansandu/eyecandy/raster/bitmap.cpp', 'source/dansandu/eyecandy/raster/bitmap.t.cpp',
                 'source/dansandu/eyecandy/geometry/mesh.t.cpp',
                 'source/dansandu/eyecandy/geometry/sphere.t.cpp',
                 'source/dansandu/eyecandy/geometry/clipping.t.cpp',
                 'source/dansandu/eyecandy/utility/string.t.cpp',
                 'source/dansandu/eyecandy/test.cpp',
                 '-I/home/udantu/workspace/eyecandy/thirdparties', '-I/home/udantu/workspace/eyecandy/source',
                 '-Werror', '-Wall', '-Wextra'])

        execute(['./target/eyecandy-test.exe'])

    execute(['scan-build', '-o', 'target/clang-analyzer', 'g++', '-std=c++1z', '-O3', '-o', 'target/eyecandy-main.exe',
             'source/dansandu/eyecandy/raster/bitmap.cpp',
             'source/dansandu/eyecandy/main.cpp',
             '-I/home/udantu/workspace/eyecandy/thirdparties', '-I/home/udantu/workspace/eyecandy/source',
             '-Werror', '-Wall', '-Wextra',
             '-lsfml-graphics', '-lsfml-window', '-lsfml-system'])
