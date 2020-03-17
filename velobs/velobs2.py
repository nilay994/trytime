# pseudo code: simulate basic velocity obstacle for 2 robots only

import matplotlib.pyplot as plt
import numpy as np

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

# return relative matrix
def rel_mat(robot_idx, num_robots):
    relmat = np.zeros((num_robots, num_robots))
    # strange numpy column assignment
    relmat[:, robot_idx] = np.ones(num_robots)
    relmat = relmat - np.identity(num_robots)
    return relmat

# populate position and velocities of all robots in a matrix
def pos_vel_mat(robot_obj_list):
    posmat = np.zeros((len(robot_obj_list), 2))
    velmat = np.zeros((len(robot_obj_list), 2))

    for i in range(len(robot_obj_list)):
        posmat[i,:] = [robot_obj_list[i].pos[0], robot_obj_list[i].pos[1]]
        velmat[i,:] = polar2cart(robot_obj_list[i].vel, robot_obj_list[i].head)

    return posmat, velmat

# break isolation, share amongst the instances :p
def detect(robot_obj_list):
    num_robots = len(robot_obj_list)

    # build matrix 
    posmat, velmat = pos_vel_mat(robot_obj_list)

    drel_norm = np.zeros((num_robots, num_robots))
    vrel_norm = np.zeros((num_robots, num_robots))
    tcpa = np.zeros((num_robots, num_robots))
    dcpa = np.zeros((num_robots, num_robots))

    for i in range(num_robots):
        relmat = rel_mat(i, num_robots)
        # relative vel and pos with each robot (3 x 2)
        drel = relmat * posmat
        vrel = relmat * velmat
        # print (i, drel, vrel)
        
        for j in range(num_robots):
            if j!=i:
                norm_vrel = np.linalg.norm(vrel[j,:])
                if norm_vrel < 0.1:
                    norm_vrel = 0.1
                tcpa[i,j] = - np.inner(drel[j,:], vrel[j,:]) / (norm_vrel**2)
                dcpa[i,j] = (abs((np.linalg.norm(drel[j,:])**2) - ((tcpa[i,j]**2) * (norm_vrel**2)))) ** (1./2)
                # print (i,j)
    print('----------------------')
    print(tcpa)
    print(dcpa)
    print('----------------------')
    # n x n symmetric matrix is now populated 
    
            
        
# def resolve(robot_obj_list, occupancy_map):

# def detect(robot_a, robot_b):
#     drel = robot_a.pos - robot_b.pos
#     vrel = polar2cart(robot_a.vel, robot_a.head) - polar2cart(robot_b.vel, robot_b.head)
#     norm_vrel = np.linalg.norm(vrel)
#     if norm_vrel < 0.1:
#         norm_vrel = 0.1
#     tcpa = - np.inner(drel, vrel) / (norm_vrel**2)
#     dcpa = (abs((np.linalg.norm(drel)**2) - ((tcpa ** 2) * (np.linalg.norm(vrel)**2)))) ** (1./2)
#     print(round(tcpa, 5), round(dcpa,5))

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

    robot_a = robot(np.array([10.0, -10.0]), 2, np.deg2rad(135))
    robot_b = robot(np.array([-12.0, 8.0]), 2, np.deg2rad(-45))
    
    robot_obj_list.append(robot_a)
    robot_obj_list.append(robot_b)

    for i in range(20):
        # plt.cla()
        detect(robot_obj_list)
        plt.plot(block = 'False')
        robot_a.move()
        robot_b.move()
        robot_a.draw(plt)
        robot_b.draw(plt)
        plt.plot()
        plt.pause(0.1)

if __name__ == "__main__":
    main()