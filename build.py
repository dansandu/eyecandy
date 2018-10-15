import os
import subprocess
import argparse
from os.path import basename, join as join_paths


def execute(command):
    print(' '.join(command))
    process = subprocess.run(command, shell = True)
    if process.returncode is not 0:
        raise RuntimeError(f'exited with status code {process.returncode}')


def list_files_recursively(directory):
    result = []
    for path, _, files in os.walk(directory):
        result.extend([join_paths(path, f) for f in files])
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Simple build command')
    parser.add_argument('--skip-tests', dest='skip_tests', action='store_true', default=False)
    args = parser.parse_args()
    
    project_directory = os.getcwd()
    main_application_source = join_paths(project_directory, 'source', 'dansandu', 'eyecandy', 'main.cpp')
    test_application_source = join_paths(project_directory, 'source', 'dansandu', 'eyecandy', 'test.t.cpp')
    main_application_target = join_paths(project_directory, 'target', 'eyecandy.exe')
    test_application_target = join_paths(project_directory, 'target', 'test-eyecandy.exe')
    extra_include_paths = [join_paths(project_directory, 'thirdparties', 'includes'), join_paths(project_directory, 'source')]    
    extra_library_paths = [join_paths(project_directory, 'thirdparties', 'libraries')]
    extra_libraries = ['sfml-graphics', 'sfml-window', 'sfml-system']
    reports_directory = join_paths(project_directory, 'target', 'clang-analyzer')

    files = list_files_recursively(join_paths(project_directory, 'source'))
    main_header_files = list(filter(lambda f: f.endswith('.hpp'), files))
    main_source_files = list(filter(lambda f: f.endswith('.cpp') and not f.endswith('.t.cpp') and not f.endswith(basename(main_application_source)), files))
    test_source_files = list(filter(lambda f: f.endswith('.t.cpp') and not f.endswith(basename(test_application_source)), files))

    execute(['clang-format', '-i', '-style=file'] + main_header_files + main_source_files + [main_application_source] + test_source_files + [test_application_source])
    if not args.skip_tests:
        execute(['scan-build', '-o', reports_directory, 'g++', '-o', test_application_target, '-O3', '-std=c++1z', '-Werror', '-Wall', '-Wextra'] +
                ['-I' + extra_include_path for extra_include_path in extra_include_paths] +
                main_source_files + test_source_files + [test_application_source])
        execute([test_application_target])
    execute(['scan-build', '-o', reports_directory, 'g++', '-o', main_application_target, '-O3', '-std=c++1z', '-Werror', '-Wall', '-Wextra'] +
            ['-I' + extra_include_path for extra_include_path in extra_include_paths] +
            ['-L' + extra_library_path for extra_library_path in extra_library_paths] +
            main_source_files + [main_application_source] +
            ['-l' + extra_library for extra_library in extra_libraries])
