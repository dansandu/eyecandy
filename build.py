import os
import subprocess


def execute(command):
    print(' '.join(command))
    process = subprocess.run(command)
    if process.returncode is not 0:
        raise RuntimeError(f'exited with status code {process.returncode}')

if __name__ == '__main__':
    files_to_format = []
    for path, _, files in os.walk('source'):
        source_files = [os.path.join(path, f) for f in files if f.endswith('.cpp') or f.endswith('.hpp')]
        files_to_format.extend(source_files)

    execute(['clang-format', '-i', '-style=file'] + files_to_format)

    execute(['g++', 'source/dansandu/eyecandy/math/matrix.t.cpp',
             'source/dansandu/eyecandy/math/transformation.t.cpp',
             'source/dansandu/eyecandy/raster/image.t.cpp',
             'source/dansandu/eyecandy/raster/bitmap.cpp', 'source/dansandu/eyecandy/raster/bitmap.t.cpp',
             'source/dansandu/eyecandy/test.cpp',
             '-I/home/udantu/workspace/eyecandy/thirdparties', '-I/home/udantu/workspace/eyecandy/source',
             '-Werror', '-Wall', '-Wextra',
             '-o', 'target/eyecandy-test.exe'])

    execute(['./target/eyecandy-test.exe'])

    execute(['g++', 'source/dansandu/eyecandy/raster/bitmap.cpp', 'source/dansandu/eyecandy/main.cpp',
             '-I/home/udantu/workspace/eyecandy/thirdparties', '-I/home/udantu/workspace/eyecandy/source',
             '-Werror', '-Wall', '-Wextra',
             '-o', 'target/eyecandy-main.exe'])
