# pseudo code: simulate basic velocity obstacle for 2 robots only

import matplotlib.pyplot as plt
from matplotlib import animation
import numpy as np
# from PyQt5.QtCore import QThread
# from PyQt5.QtCore import QTimer
# from PyQt5.QtCore import pyqtSignal
# from PyQt5.QtWidgets import (QApplication, QLabel, QWidget)

MAX_VEL = 10
MAX_DIST = 25
RR = 2
MAX_ROBOTS = 3

def config_matplotlib():
    plt.rcParams['figure.figsize'] = (10, 8)
    gray = "444444"
    plt.rcParams['axes.facecolor'] = 'f5f5f5'
    plt.rcParams['axes.edgecolor'] = gray
    plt.rcParams['grid.linestyle'] = '-'
    plt.rcParams['grid.alpha'] = 0.8
    plt.rcParams['grid.color'] = 'white'
    plt.rcParams['grid.linewidth'] = 2
    plt.rcParams['axes.axisbelow'] = True
    plt.rcParams['axes.labelcolor'] = gray
    plt.rcParams['text.color'] = gray
    plt.rcParams['xtick.color'] = gray
    plt.rcParams['ytick.color'] = gray
    

# dir = angle in radians wrt +x
def polar2cart(mag, direction):
    x = mag * np.cos(direction)
    y = mag * np.sin(direction)
    cart = np.array([x, y])
    return cart

class robot:
    def __init__(self, pos, vel, head):
        self.pos = pos
        self.vel = vel
        self.head = head

    def move(self):
        [delx, dely] = polar2cart(self.vel*0.2, self.head)
        self.pos[0] += delx
        self.pos[1] += dely

    def draw(self, plt):
        dx, dy = polar2cart(self.vel/20, self.head)
        originpt = self.pos[0], self.pos[1]
        # print (originpt)
        plt.quiver(*originpt, dx, dy, scale = 10)

# # return relative matrix
# def rel_mat(robot_idx):
#     relmat = np.zeros((MAX_ROBOTS, MAX_ROBOTS))
#     relmat(:, robot_idx) = np.ones((MAX_ROBOTS, 1))
#     relmat = relmat - np.diagonal(3)
#     return relmat

# # populate position and velocities of all robots in a matrix
# def pos_vel_mat(robot_obj_list):
#     posmat = np.zeros((MAX_ROBOTS, 2))
#     for each robot in robot_obj_list:
#         posmat[i,:] = [robot_obj_list[i].pos[0], robot_obj_list[i].pos[1]]
    
#     velmat = np.zeros((MAX_ROBOTS, 2))
#     for each robot in robot_obj_list:
#         velmat[i, :] = [robot_obj_list[i].vel[0], robot_obj_list[i].vel[1]]

#     return posmat, velmat

# # break isolation, share amongst the instances :p
# def detect(robot_obj_list):
#     # build matrix 
#     posmat, velmat = pos_vel_mat(robot_obj_list)

#     d_rel_norm = np.zeros((MAX_ROBOTS, MAX_ROBOTS))
#     v_rel_norm = np.zeros((MAX_ROBOTS, MAX_ROBOTS))
#     for each robot: 
#         relmat = rel_mat(i)
#         # relative vel and pos with each robot (3 x 2)
#         d_rel = relmat * posmat
#         v_rel = relmat * velmat
#         # row-wise norm (3 x 3)
#         d_rel_norm(i,:) = np.sum(np.abs(d_rel)**2,axis=-1)**(1./2)
#         v_rel_norm(i,:) = np.sum(np.abs(v_rel)**2,axis=-1)**(1./2)
#         t_cpa(i, :) = 
#     # 3 x 3 symmetric matrix is now populated 
#     #  

#     # find d_rel and v_rel for each robot i
#     for robot i in robot_obj_list:
#         while (selected_robot != i):
#             local_ctr = 0
#             d_rel[local_ctr] = (roboti.pos - robotcurrent.pos)
#             v_rel[local_ctr] = (roboti.vel - robotcurrent.vel)
#             tcpa[local_ctr] = - np.inner(drel, vrel) / (np.norm(vrel)**2)
#             # if less than one second, robots don't have time!! change alt?
#             if tcpa[local_ctr] < 1:
#                 occupancy_map[i][local_ctr] = 1
                

#             # if relative less than robot radius, robots are too close!
#             if dcpa[local_ctr] < RR:
#                 occupancy_map[i][local_ctr] = 1

#             local_ctr = local_ctr + 1
#             selected_robot = selected_robot + 1
            
        
# def resolve(robot_obj_list, occupancy_map):

def detect(robot_a, robot_b):
    drel = robot_a.pos - robot_b.pos
    vrel = polar2cart(robot_a.vel, robot_a.head) - polar2cart(robot_b.vel, robot_b.head)
    tcpa = - np.inner(drel, vrel) / (np.linalg.norm(vrel)**2)
    dcpa = (abs((np.linalg.norm(drel)**2) - ((tcpa ** 2) * (np.linalg.norm(vrel)**2)))) ** (1./2)
    print(round(tcpa, 2), round(dcpa,5))

robot_obj_list = []
def main():
    config_matplotlib()
    gray = "444444"
    fig = plt.figure()
    plt.grid(True, which='major')
    plt.grid(True, which='minor', color=gray, linestyle = '--', linewidth = 0.5)
    plt.minorticks_on()
    plt.axis('equal')
    plt.xlim(-30, 30)
    plt.ylim(-30, 30)

    robot_a = robot(np.array([5.0, 0.1]), 2, np.deg2rad(180))
    robot_b = robot(np.array([-5.0, 0.1]), 2, np.deg2rad(0))
    
    robot_obj_list.append(robot_a)
    robot_obj_list.append(robot_b)

    for i in range(18):
        # plt.cla()
        detect(robot_a, robot_b)
        plt.plot(block = 'False')
        robot_a.move() #2, np.arctan(-2))
        robot_b.move() #2, np.arctan(1))
        robot_a.draw(plt)
        robot_b.draw(plt)
        # print('---------------------------------')
        # print(robot_a.pos[0], robot_a.pos[1])
        # print(robot_b.pos[0], robot_b.pos[1])
        # print('---------------------------------')
        plt.plot()
        plt.pause(0.1)
        # input("Press [enter] to continue.")

if __name__ == "__main__":
    main()