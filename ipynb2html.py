import subprocess, os, shutil, argparse

def convert_course(course):
    directory = os.path.join("./notebook", course)
    if not os.path.isdir(directory):
        raise FileNotFoundError("Cannot find " + os.path.abspath(directory))
    assets_dir = os.path.join(directory, "assets")
    output_dir = os.path.join("./docs/courses", f"_{course}")
    os.makedirs(output_dir, exist_ok=True)

    if os.path.isdir(assets_dir) and not os.path.isdir(os.path.join(output_dir, "assets")):
        shutil.copytree(assets_dir, os.path.join(output_dir, "assets"))
        
    files = [x for x in os.listdir(directory) if x.endswith(".ipynb")]
    for file in files:
        input_path = os.path.join(directory, file)
        output_name = file.replace(".ipynb", ".md")
        output_path = os.path.join(output_dir, output_name)
        bash_command = [
            'jupyter', 'nbconvert',
            f'--output-dir={output_dir}',
            '--template=./base.tpl',
            '--to=html',
            '--config=configs.py',
            input_path
        ]
        subprocess.run(bash_command)

parser = argparse.ArgumentParser(description='Convert a course notebook directory to jekyll collection')
parser.add_argument('--course', type=str,
                    help='The course code')

args = parser.parse_args()
convert_course(args.course)