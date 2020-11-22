# The code for plotting the illustraions 


import plotly.express as px
import plotly.graph_objects as go
import numpy as np

def rodrigues(k, v, theta):
    k /= np.linalg.norm(k)
    cross = np.cross(k, v)
    cos = np.cos(theta)[:, None]
    sin = np.sin(theta)[:, None]
    dot = np.dot(k, v)
    v_p = np.tile(v, (len(theta), 1))
    k_p = np.tile(k, (len(theta), 1))
    return v_p * cos + sin * cross  +  dot * k_p * (1-cos)
    
def plot(output_file):
    axis = np.random.random(3)
    axis /= np.linalg.norm(axis)
    vector = np.random.random(3)
    angle = np.linspace(0, 2 *np.pi, 100)
    dt = 0.2
    points = rodrigues(axis, vector, angle)
    alpha = vector.dot(axis) / np.linalg.norm(axis) * axis 
    d = np.cross(axis, vector) / np.linalg.norm(vector)
    derivative = vector + dt * d

    trajectory = go.Scatter3d(x=points[:, 0], y=points[:, 1], z=points[:, 2], 
                              mode="lines", name="x trajectory")
    plot_axis = go.Scatter3d(x=[0, axis[0]],y=[0, axis[1]],z=[0, axis[2]], 
                             name="a_vec")
    plot_vector = go.Scatter3d(x=[0, vector[0]],y=[0, vector[1]],z=[0, vector[2]], 
                               name="x")
    plot_normal = go.Scatter3d(x=[alpha[0], vector[0]], y=[alpha[1], vector[1]], z=[alpha[2], vector[2]], 
                               name="trajectory normal", mode="lines", line=dict(dash="dot"))
    plot_y = go.Scatter3d(x=[vector[0], derivative[0], 0], y=[vector[1], derivative[1], 0], z=[vector[2], derivative[2], 0], 
                          name="y")
    fig = go.Figure(data=[trajectory, plot_axis, plot_vector, plot_normal, plot_y])
    fig.update_layout(margin=dict(l=0, r=0, b=0, t=0), 
                      scene_aspectmode='cube',
                      legend=dict(x=0, y=0))
    fig.write_html(output_file, full_html=False, auto_open=False, include_plotlyjs="cdn", auto_play=False)