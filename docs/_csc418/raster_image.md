---
title: Raster Image
order: 1
---

```python
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import cv2
```

# Image
Consider image as function
$$I(x, y): R\subset \mathbb R^2 \rightarrow V$$
where $R$ is the area (in most cases rectangle area) and $V$ is the set of possible pixel values.   
For example, for a grayscale image, $V= \mathbb R^+$, a.k.a. the brightness.  
For a idealized color image, with RGB values at each pixel, then $V=(\mathbb R^3)^+$

A raster iamge is then the sample of the continuous image. Each pixel is a sample and the rectangular domain of a $W\times H$ image is 
$$R = [-0.5, W-0.5]\times [-0.5, H-0.5]$$


```python
img = np.random.rand(3, 4)
plt.imshow(img, cmap="Reds")
plt.axhline(0)
plt.axvline(0);
```


    
![png](raster_image_files/raster_image_3_0.png)
    


# Pixels and subpixels
The raw color measurements made by modern digital cameras are typically stored with a single color channel per pixel. This information is stored as a seemingly 1-channel image, but with an understood convention for interpreting each pixel as the red, green or blue intensity value given some pattern. The most common is the Bayer pattern. In this assignment, we'll assume the top left pixel is green, its right neighbor is blue and neighbor below is red, and its kitty-corner neighbor is also green.


To demosaic an image, we would like to create a full rgb image without downsampling the image resolution. So for each pixel, we'll use the exact color sample when it's available and average available neighbors (in all 8 directions) to fill in missing colors. This simple linear interpolation-based method has some blurring artifacts and can be improved with more complex methods.


```python
bayer_filter = np.zeros((10, 20, 3))
bayer_filter[::2, ::2, 1] = 1
bayer_filter[1::2, 1::2, 1] = 1
bayer_filter[::2, 1::2, 0] = 1
bayer_filter[1::2, ::2, 2] = 1
plt.imshow(bayer_filter)
plt.title("Bayer Filter");
```


    
![png](raster_image_files/raster_image_5_0.png)
    



```python
def simulate_bayer_filter(image):
    output = np.empty(image.shape[:2])
    output[::2, ::2] = image[::2, ::2, 1] 
    output[1::2, 1::2] = image[1::2, 1::2, 1]
    output[::2, 1::2] = image[::2, 1::2, 0]
    output[1::2, ::2] = image[1::2, ::2, 2]
    return output
```


```python
def demosaic(image):
    
    corner_kernel = np.array([[.25, 0, .25], 
                              [0,   0, 0], 
                              [.25, 0, .25]])
    cross_kernel = np.array([[0.,  .25, 0. ], 
                             [.25, 0.,  .25], 
                             [0.,  .25, 0. ]])
    h_kernel = np.array([[.5, 0., .5]])
    v_kernel = h_kernel.T
    
    output = np.empty(list(image.shape) + [3])
    # fill in the R value
    # if bayer pixel is red, take self
    output[::2, 1::2, 0] = image[::2, 1::2]
    # if bayer pixel is green, 
    output[::2, ::2, 0] = cv2.filter2D(image, -1, h_kernel)[::2, ::2]
    output[1::2, 1::2, 0] = cv2.filter2D(image, -1, v_kernel)[1::2, 1::2]
    # if bayer pixel is blue, take corners
    output[1::2, ::2, 0] = cv2.filter2D(image, -1, corner_kernel)[1::2, ::2]
    
    # fill in the G value
    # if bayer pixel is red or blue, take the cross
    output[::2, 1::2, 1] = cv2.filter2D(image, -1, cross_kernel)[::2, 1::2]
    output[1::2, ::2, 1] = cv2.filter2D(image, -1, cross_kernel)[1::2, ::2]
    # if bayer pixel is green, take self
    output[::2, ::2, 1] = image[::2, ::2]
    output[1::2, 1::2, 1] = image[1::2, 1::2]
    
    # fill in the blue value
    # if bayer pixel is red, take corners
    kernel = np.array([[.25, 0, .25], 
                       [0,   0, 0], 
                       [.25, 0, .25]])
    output[::2, 1::2, 2] = cv2.filter2D(image, -1, corner_kernel)[::2, 1::2]
    # if bayer pixel is green. take top and botton neightbors
    output[::2, ::2, 2] = cv2.filter2D(image, -1, v_kernel)[::2, ::2]
    output[1::2, 1::2, 2] = cv2.filter2D(image, -1, h_kernel)[1::2, 1::2]
    # if bayer pixel is blue, take self
    output[1::2, ::2, 2] = image[1::2, ::2]
    return output.astype(np.uint8)
```


```python
image = plt.imread("./assets/yurina.jpg")
bayered = simulate_bayer_filter(image)
demosed = demosaic(bayered)

plt.figure(figsize=(18, 6))
plt.subplot(131); plt.imshow(image); plt.title("Original")
plt.subplot(132); plt.imshow(bayered, cmap="gray"); plt.title("Simulated Bayer Filter")
plt.subplot(133); plt.imshow(demosed); plt.title("Demosed Image");
```


    
![png](raster_image_files/raster_image_8_0.png)
    


# Conversion between RGB and HSV

HSV stands for hue, saturation, and value. Is another way of representing an image. Which is to have a more closely align with the way human vision perceives color-making attributes. $H\in [0, 360]$ is a periodic measurement and $S\in [0, 1], V\in[0, 1]$

## RGB to HSV
Given RGB values in $[0, 1]^3$,  
First calculate the chroma range, 
$$C_\min = \min(R, G,B), C_\max = \max(R, G, B), \Delta = C_\max - C_\min$$


__Value__
$$V = C_\max$$
$V$ is the max of lightness among the RGB channels

__Hue__
$$H = \begin{cases}
0 &\Delta = 0\\
60 (\frac{G-B}\Delta) & C_\max = R\\
60(\frac{B-R}{\Delta} + 2) &C_\max = G\\
60(\frac{R-G}{\Delta} + 4) &C_\max = B
\end{cases}$$

Note that when $C_\min = C_\max\Leftrightarrow \Delta = 0$, this means the color is black/white.  

__Saturation__ 
$$S = \begin{cases}
0 &\Delta = 0\\
\Delta / C_\max &C_\max \neq 0
\end{cases}$$

## HSV to RGB
Given $H\in [0, 360), S, V \in [0,1]$ (Note that since $H$ is periodic, if $H\in \mathbb R$, just take mod of 360)  


$$\begin{align*}
C &= V\cdot S\\
X &= C(1-|(\frac{H}{60}\mod 2) - 1|)\\
m &= V - C\\
RGB_{raw} &= \begin{cases}
(C, X, 0) &H\in [0, 60)\\
(X, C, 0) &H\in [60, 120)\\
(0, C, X) &H\in [120, 180)\\
(0, X, C) &H\in [180, 240)\\
(X, 0, C) &H\in [240, 300)\\
(C, 0, X) &H\in [300, 360)\\
\end{cases}\\
RGB &= 255(RGB_{raw} + m)
\end{align*}$$



```python
def rgb2hsv(image):
    output = np.zeros(image.shape)
    if image.dtype == np.uint8:
        image = image.astype(float) / 255
    r, g, b = image[:, :, 0], image[:, :, 1], image[:, :, 2]
    c_max = np.max(image, axis=2)
    c_min = np.min(image, axis=2)
    delta = c_max - c_min
    r_indice = np.logical_and(c_max == r, delta > 0)
    output[r_indice, 0] = 60 * ((g[r_indice] - b[r_indice]) / delta[r_indice])

    g_indice = np.logical_and(c_max == g, delta > 0)
    output[g_indice, 0] = 60 * ((b[g_indice] - r[g_indice]) / delta[g_indice] + 2)

    b_indice = np.logical_and(c_max == b, delta > 0)
    output[b_indice, 0] = 60 * ((r[b_indice] - g[b_indice]) / delta[b_indice] + 4)
    
    output[:, :, 0] = np.mod(output[:, :, 0], 360)

    output[c_max > 0, 1] = delta[c_max > 0] / c_max[c_max > 0]
    output[:, :, 2] = c_max
    return output
```


```python
def hsv2rgb(image):
    h, s, v = image[:, :, 0], image[:, :, 1], image[:, :, 2]
    c = v * s
    x = c * (1 - np.abs(np.mod(h/60, 2) - 1))
    m = v - c
    
    rgb = np.zeros(image.shape)
    
    cond = np.logical_and(h >= 0, h < 60)
    rgb[cond, 0] = c[cond]
    rgb[cond, 1] = x[cond]
    
    cond = np.logical_and(h >= 60, h < 120)
    rgb[cond, 0] = x[cond]
    rgb[cond, 1] = c[cond]
    
    cond = np.logical_and(h >= 120, h < 180)
    rgb[cond, 1] = c[cond]
    rgb[cond, 2] = x[cond]
    
    cond = np.logical_and(h >= 180, h < 240)
    rgb[cond, 1] = x[cond]
    rgb[cond, 2] = c[cond]
    
    cond = np.logical_and(h >= 240, h < 300)
    rgb[cond, 0] = x[cond]
    rgb[cond, 2] = c[cond]
    
    cond = np.logical_and(h >= 300, h <= 360)
    rgb[cond, 0] = c[cond]
    rgb[cond, 2] = x[cond]
    
    return np.round(255 * (rgb + m[:, :, np.newaxis])).astype(np.uint8)
```


```python
image = plt.imread("./assets/yurina.jpg")
hsv_image = rgb2hsv(image)
fig, axs = plt.subplots(1, 3, figsize=(14.4, 4))
sns.heatmap(hsv_image[:, :, 0], ax=axs[0], cmap="gray")
axs[0].set_title("hue")
sns.heatmap(hsv_image[:, :, 1], ax=axs[1], cmap="gray")
axs[1].set_title("saturation")
sns.heatmap(hsv_image[:, :, 2], ax=axs[2], cmap="gray")
axs[2].set_title("value");
```


    
![png](raster_image_files/raster_image_14_0.png)
    


With HSV images, we can easily tune the lightness, hue, and saturation


```python
def hue_shift(image, shift):
    output = image.copy()
    output[:, :, 0] = np.mod(output[:, :, 0] + shift, 360)
    return output

def saturate_change(image, factor):
    output = image.copy()
    output[:, :, 1] = np.clip(output[:, :, 1] * (1 + factor), 0, 1)
    return output

def lightness_change(image, factor):
    output = image.copy()
    output[:, :, 2] = np.clip(output[:, :, 2] * (1 + factor), 0, 1)
    return output
```


```python
plt.figure(figsize=(15, 15))

plt.subplot(331); plt.title("Hue shift by -30")
plt.imshow(hsv2rgb(hue_shift(hsv_image, -30)));
plt.subplot(332); plt.title("original")
plt.imshow(image);
plt.subplot(333); plt.title("Hue shift by 30")
plt.imshow(hsv2rgb(hue_shift(hsv_image, 30)));

plt.subplot(334); plt.title("Desaturate by 50%")
plt.imshow(hsv2rgb(saturate_change(hsv_image, -.5)));
plt.subplot(335); plt.title("original")
plt.imshow(image);
plt.subplot(336); plt.title("Saturate by 50%")
plt.imshow(hsv2rgb(saturate_change(hsv_image, .5)));

plt.subplot(337); plt.title("Darken by 50%")
plt.imshow(hsv2rgb(lightness_change(hsv_image, -.5)));
plt.subplot(338); plt.title("original")
plt.imshow(image);
plt.subplot(339); plt.title("Lighten by 50%")
plt.imshow(hsv2rgb(lightness_change(hsv_image, .5)));
```


    
![png](raster_image_files/raster_image_17_0.png)
    


# Alpha Compositing

If we want
to composite a foreground color $c_f$ over background color $c_b$ , and the fraction of
the pixel covered by the foreground is $\alpha$, then we can use the formula 
$$c = ac_f + (1-a) c_b$$
