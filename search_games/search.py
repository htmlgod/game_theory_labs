from math import sqrt
import numpy as np
import matplotlib.pyplot as plt
import random
from mpl_toolkits.mplot3d import Axes3D

def gen_fibonacci_sphere_points(samples=500, radius=1.0):
    phi = np.pi * (3. - np.sqrt(5.))  # golden angle in radians
    points = np.zeros((samples, 3))

    for i in range(samples):
        y = 1 - (i / float(samples - 1)) * 2  # y goes from 1 to -1
        height = radius * y  # height at y
        radius_at_height = np.sqrt(radius**2 - height**2)  # radius at height

        theta = phi * i  # golden angle increment

        x = np.cos(theta) * radius_at_height
        z = np.sin(theta) * radius_at_height

        points[i] = [x, height, z]

    return points

def create_cylinder_mesh(center_x,center_y,radius,height_z):
    z = np.linspace(-height_z/2, height_z/2, 50)
    theta = np.linspace(0, 2*np.pi, 50)
    theta_grid, z_grid=np.meshgrid(theta, z)
    x_grid = radius*np.cos(theta_grid) + center_x
    y_grid = radius*np.sin(theta_grid) + center_y
    return x_grid,y_grid,z_grid

def make_points_projection_on_cylinder_surface(sphere_points, cylinder_h, cylinder_r):
    for point in sphere_points:
        # above and below cylinder
        if abs(point[2]) <= cylinder_h/2:
            sphere_section_radius = sqrt(point[1]**2 + point[0]**2)
            k = cylinder_r / sphere_section_radius
            point[0] *= k
            point[1] *= k
        else:
            if point[2] > cylinder_h/2:
                point[2] = cylinder_h/2
            if point[2] < -cylinder_h/2:
                point[2] = -cylinder_h/2

def make_a_shot(radius, height):
    z = random.uniform(-height/2, height/2)
    if abs(z) == height/2:
        y = random.uniform(-radius, radius)
        x = random.uniform(-radius, radius)
        while x**2 + y**2 > radius**2:
            y = random.uniform(-radius, radius)
            x = random.uniform(-radius, radius)
        print(f'${x=},{y=}, {x**2 + y**2} <= {radius**2}')
        return x,y,z
    else:
        y = random.uniform(-radius, radius)
        x = random.uniform(-radius, radius)
        while abs(x**2 + y**2 - radius**2) > 0.001:
            #print(f'{x=},{y=}, {x**2 + y**2} != {radius**2}')
            y = random.uniform(-radius, radius)
            x = random.uniform(-radius, radius)
        #print(f'$${x=},{y=}, {x**2 + y**2} = {radius**2}')
        return x,y,z

def check_shot(c_point, shot, eps):
    return sqrt((c_point[0] - shot[0])**2 + (c_point[1] - shot[1])**2 + (c_point[2] - shot[2])**2) <= eps

def solve_analytical(dots_number, height, radius, eps):
    cylinder_flat_area = radius**2 * np.pi
    cylinder_curve_area = 2*radius*height*np.pi
    gc = (dots_number*2*eps**2*np.pi) / (2*cylinder_flat_area + cylinder_curve_area)
    return 1 if gc > 1 else gc

if __name__ == "__main__":
    # game params
    number_of_points = 50
    cylinder_height = 10
    cylinder_radius = 3
    attempts_amount = 10
    sphere_radius = sqrt(cylinder_radius**2 + cylinder_height**2/4)
    epsilon = 1.2
    print(f'Game cost by analitic solution:'
          f'{solve_analytical(number_of_points, cylinder_height, cylinder_radius, epsilon)}')

    # Preparation: gen fibonacci points on sphere, project them on cylinder
    points = gen_fibonacci_sphere_points(number_of_points, sphere_radius)
    make_points_projection_on_cylinder_surface(points, cylinder_height, cylinder_radius)
    
    # Draw
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(points[:, 0], points[:, 1], points[:, 2], s=2, color='red')
    Xc,Yc,Zc = create_cylinder_mesh(0,0,cylinder_radius,cylinder_height)
    ax.plot_surface(Xc, Yc, Zc, alpha=0.2, color='yellow')
    
    # player B moves
    if attempts_amount != 0:
        total_wins = 0;
        for i in range(0, attempts_amount):
            shot = make_a_shot(cylinder_radius, cylinder_height)
            for p in points:
                if check_shot(p, shot, epsilon):
                    total_wins += 1
                    ax.scatter(shot[0], shot[1], shot[2], s=9, color='blue')
        print(f'Game cost by numeric solution: {total_wins/attempts_amount}')

    #epsilon spheres (areas)
    for p in points:
        # draw sphere
        u, v = np.mgrid[0:2*np.pi:20j, 0:np.pi:10j]
        x = np.cos(u)*np.sin(v)*epsilon + p[0]
        y = np.sin(u)*np.sin(v)*epsilon + p[1]
        z = np.cos(v)*epsilon + p[2]
        # alpha controls opacity
        ax.plot_surface(x, y, z, color="red", alpha=0.4) #alpha = 0.3
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('Search game on cylinder')
    ax.set_box_aspect((2 * cylinder_radius, 2 * cylinder_radius, cylinder_height))
    plt.show()
