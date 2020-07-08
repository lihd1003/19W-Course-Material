import cv2, json
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sn
from numpy import ma

# helper functions

def read_json(file_name):
    with open(file_name, "r") as f:
        data = json.load(f)
        camera, objects = data['camera'], data['objects']
        camera = Camera(**camera)
        objs = []
        for obj in objects:
            if obj['type'] == 'plane':
                objs.append(Plane(p=obj['point'], n=obj['normal']))
            elif obj['type'] == 'sphere':
                objs.append(Sphere(r=obj['radius'], c=obj['center']))
            elif obj['type'] == 'triangle':
                objs.append(Triangle(obj['corners']))
        return camera, objs
    
            
def normalize(vec):
    return vec / np.linalg.norm(vec)

def visualize(id_map, depth, normal):
    plt.figure(figsize=(16, 9))
    plt.subplot(221); plt.axis("off"); plt.title("Object ID")
    plt.imshow(id_map, cmap="Greens")
    plt.subplot(222); plt.axis("off"); plt.title("Depth")
    sn.heatmap(depth, cmap="Greens", square=True)
    plt.subplot(223); plt.axis("off"); plt.title("Normal")
    plt.imshow((normal + 1) / 2)
    
# Camera stores all the required parameters 
class Camera:
    def __init__(self, 
                 eye, 
                 up, look, 
                 focal_length, 
                 height, width, 
                 type):
        """
        e:             eye / viewing point
        up:            the upward direction of the view
        look:          the viewing direction
        d:             image plane distance / focal length
        height, width: width and height of image plane
        type:          perspective or orthographic
        """
        
        self.e = np.array(eye)
        self.v = normalize(np.array(up))
        self.w = normalize(-np.array(look))
        self.u = np.cross(self.v, self.w)
        self.d, self.height, self.width = focal_length, height, width
        self.type = type
        
def generate_ray_map_perspective(camera, width, height):
    """
    Generate a width * height * 3 array
    where each (i, j) is the direction of the viewing ray
    """
    ray_map = np.empty((width, height, 3))
    i = np.arange(width)
    j = np.arange(height)
    u = camera.width * (i + 0.5) / width - camera.width / 2
    v = camera.height / 2 - camera.height * (j + 0.5) / height
    vv, uu = np.meshgrid(v, u)
    ray_map[:, :, 0] = uu * camera.u[0] + vv * camera.v[0] - camera.d * camera.w[0]
    ray_map[:, :, 1] = uu * camera.u[1] + vv * camera.v[1] - camera.d * camera.w[1]
    ray_map[:, :, 2] = uu * camera.u[2] + vv * camera.v[2] - camera.d * camera.w[2]
    return ray_map, np.tile(camera.e, (width, height, 1))

# The abstract class for different types of object-ray intersection
class Object:
    def intersect(self, ray_d, ray_e, min_t):
        """ return t and normal maps
        of the intersection, if not intersect, 
        then the position will have NaN
        """
        raise NotImplementedError
    
    def add_material(self, material):
        self.material = material


class Triangle(Object):
    def __init__(self, corners):
        """ corners is a 3 * 3 list of list 
        each row is a corner
        """
        self.corners = np.array(corners)
    
    def intersect(self, ray_d, ray_e, min_t):
        abc = self.corners[0] - self.corners[1]
        def_ = self.corners[0] - self.corners[2]
        jkl = self.corners[0][None, None, :] - ray_e
        
        def_x_ghi = np.cross(def_, ray_d)
        M = abc[0] * def_x_ghi[:, :, 0] + \
            abc[1] * def_x_ghi[:, :, 1] + \
            abc[2] * def_x_ghi[:, :, 2] 
        
        abc_x_jkl = np.cross(abc, jkl)
        
        time =  - (def_[0] * abc_x_jkl[:, :, 0] + \
                def_[1] * abc_x_jkl[:, :, 1] + \
                def_[2] * abc_x_jkl[:, :, 2] ) / M
        
        gamma = (ray_d[:, :, 0] * abc_x_jkl[:, :, 0] + \
                 ray_d[:, :, 1] * abc_x_jkl[:, :,1] + \
                 ray_d[:, :, 2] * abc_x_jkl[:, :, 2] ) / M
        
        beta = (jkl[:, :, 0] * def_x_ghi[:, :, 0] + \
                jkl[:, :, 1] * def_x_ghi[:, :, 1] + \
                jkl[:, :, 2] * def_x_ghi[:, :, 2] ) / M
        
        t = time
        t[(time < min_t) | \
          (gamma < 0) | (gamma > 1) | \
          (beta < 0) | (beta > 1) | \
          (beta + gamma > 1)] = np.nan
        
        n = normalize(np.cross(abc, def_))
        return t, n

class Plane(Object):
    def __init__(self, p, n):
        """
        p: an arbitrary point on the plane
        n: the normal of the plane
        """
        self.p = np.array(p)
        self.n = np.array(n)
    
    def intersect(self, ray_d, ray_e, min_t):
    
        # dot product of each ray_direction and normal
        denom = ray_d[:, :, 0] * self.n[0] + \
                ray_d[:, :, 1] * self.n[1] + \
                ray_d[:, :, 2] * self.n[2]
        p_map = self.p[None, None, :]
        t = (p_map - ray_e)[:, :, 0] * self.n[0] + \
            (p_map - ray_e)[:, :, 1] * self.n[1] + \
            (p_map - ray_e)[:, :, 2] * self.n[2] 
        t = t / denom
        t[t < min_t] = np.nan
        n = np.tile(normalize(self.n), list(t.shape) + [1])
        return t, n   
    
class Sphere(Object):
    def __init__(self, c, r):
        """
        c: center of the sphere
        r: radius
        """
        self.c = np.array(c)
        self.r = r
        
    def intersect(self, ray_d, ray_e, min_t):
        c_map = self.c[None, None, :]
        A = ray_d[:, :, 0] * ray_d[:, :, 0] + \
            ray_d[:, :, 1] * ray_d[:, :, 1] + \
            ray_d[:, :, 2] * ray_d[:, :, 2]
        B = 2 * (ray_d[:, :, 0] * (ray_e - c_map)[:, :, 0] + \
                 ray_d[:, :, 1] * (ray_e - c_map)[:, :, 1] + \
                 ray_d[:, :, 2] * (ray_e - c_map)[:, :, 2])
        C =  (ray_e - c_map)[:, :, 0] ** 2 + \
             (ray_e - c_map)[:, :, 1] ** 2 + \
             (ray_e - c_map)[:, :, 2] ** 2 - self.r ** 2
        
        discriminant = B * B - 4 * A * C
        sqrt_dis = np.sqrt(discriminant, 
                           where=discriminant > 0)
        t1 = (- B + sqrt_dis) / (2 * A)
        t2 = (- B - sqrt_dis) / (2 * A)
        
        t = t1
        t[discriminant < 0] = np.nan
        with np.errstate(invalid='ignore'):
            t[(t1 < min_t) & (t2 < min_t)] = np.nan
            t[(t2 < t1) & (t2 >= min_t)] = t2[(t2 < t1) & (t2 >= min_t)]
        n = ray_e + \
            np.repeat(t[:, :, np.newaxis], 3, axis=2) * ray_d - \
            c_map
        n /= self.r
        return t, n