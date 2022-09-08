Import('env', 'projenv')

import os
import glob
import gzip
from shutil import copytree, ignore_patterns
from css_html_js_minify import html_minify, js_minify, css_minify

def gzip_file(file_dst, content):
    # Crea un nuovo file.gz e aprilo in scrittura 'testuale'
    with gzip.open(file_dst+'.gz', 'wt') as f_out:
        f_out.write(content)

def minify_file(file_src, ext):
    with open(file_src, 'r') as f_in:
        # Prova a minimizzare il file
        if ext=='css':
            return css_minify(f_in.read())
        elif ext=='js':
            return js_minify(f_in.read())
        elif ext=='html':
            return html_minify(f_in.read())
        # Altrimenti restituisci solo il contenuto
        else:
            return f_in.read()

def compress_webfiles(source, target, env):
    project_dir = env.get("PROJECT_DIR")
    webui_src = os.path.join(project_dir, 'html')
    filetypes_to_gzip = [ 'css', 'html', 'js', 'svg']

    # Elenca tutti i file da comprimere
    for ext in filetypes_to_gzip:
        for file in glob.glob(webui_src+'/**/*.'+ext, recursive=True):
            # Minimizza il file
            f_minified = minify_file(file, ext)
            print(file+' minified')

            # Comprimi il risultato e scrivi il file su disco
            gzip_file(file, f_minified)
            print(file+' gzipped')

    # Copia la directory escludendo i file di cui esiste una versione compressa
    copytree(
        webui_src,
        os.path.join(project_dir, 'data', 'html'),
        ignore=ignore_patterns(*['*.'+ext for ext in filetypes_to_gzip]),
        dirs_exist_ok=True
    )

env.AddPreAction('$BUILD_DIR/spiffs.bin', compress_webfiles)
