from jinja2 import Template
from IPython.display import HTML

OBJViewers = []

def OBJViewer(name, obj, texture):
    OBJViewers.append({
        'name': name,
        'obj': obj,
        'texture': texture
    })
    display(HTML(
        f"""
        <canvas id="{name}" style="height:100%; width:60%; display:block"></canvas>
        """
    ))
    
def render():
    f = open("./assets/meshes/OBJViewer.tpl", "r")
    template = Template(f.read())
    r = template.render(OBJViewers=OBJViewers)
    display(HTML(r))