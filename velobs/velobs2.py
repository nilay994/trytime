# pseudo code: simulate basic velocity obstacle for 2 robots only

import matplotlib.pyplot as plt
import numpy as np

MAX_VEL = 20
MAX_DIST = 25
RR = 4
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

def cart2polar(vel_vec):
    mag = np.linalg.norm(vel_vec)
    direction = np.arctan2(vel_vec[1], vel_vec[0])
    return mag, direction

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
        # create copies for after conflict resolution
        self.oldvel = vel
        self.oldhead = head

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
        posmat[i,:] = np.array([robot_obj_list[i].pos[0], robot_obj_list[i].pos[1]])
        velmat[i,:] = polar2cart(robot_obj_list[i].vel, robot_obj_list[i].head)

    return posmat, velmat

# break isolation, share amongst the instances :p
def detect2(robot_obj_list):
    num_robots = len(robot_obj_list)

    # build matrix 
    posmat, velmat = pos_vel_mat(robot_obj_list)

    drel_norm = np.zeros((num_robots, num_robots))
    vrel_norm = np.zeros((num_robots, num_robots))
    tcpa = np.zeros((num_robots, num_robots))
    dcpa = np.zeros((num_robots, num_robots))

    i = 0
    relmat = rel_mat(i, num_robots)
    # relative vel and pos with each robot (3 x 2)
    drel = relmat * posmat
    vrel = relmat * velmat
    # print ('-------------------------')
    # print (i)
    # print (drel)
    # print (vrel)
    # print ('-------------------------')
    
    for j in range(num_robots):
        if j!=i:
            norm_vrel = np.linalg.norm(vrel[j,:])
            if norm_vrel < 0.1:
                norm_vrel = 0.1
            tcpa[i,j] = - np.inner(drel[j,:], vrel[j,:]) / (norm_vrel**2)
            dcpa[i,j] = (abs((np.linalg.norm(drel[j,:])**2) - ((tcpa[i,j]**2) * (np.linalg.norm(vrel[j,:])**2)))) ** (1./2)

    print('----------------------')
    print(np.round(tcpa,2))
    print(np.round(dcpa,2))
    print('----------------------')
    # n x n symmetric matrix is now populated 

# def resolve(robot_obj_list, occupancy_map):

# hacky direct version
def detect(robot_a, robot_b):
    drel = robot_a.pos - robot_b.pos
    vrel = polar2cart(robot_a.vel, robot_a.head) - polar2cart(robot_b.vel, robot_b.head)
    norm_vrel = np.linalg.norm(vrel)
    if norm_vrel < 0.1:
        norm_vrel = 0.1
    tcpa = - np.inner(drel, vrel) / (norm_vrel**2)
    dcpa = (abs((np.linalg.norm(drel)**2) - ((tcpa ** 2) * (np.linalg.norm(vrel)**2)))) ** (1./2)
    if (tcpa > 0) and (dcpa < 0.5):
        newvela = resolve(robot_a, robot_b)
        avel, robot_a.head = cart2polar(newvela)
        robot_a.vel = np.clip(avel, -2, 2)
        # print (robot_a.vel, np.rad2deg(robot_a.head))
    else: 
        robot_a.vel = robot_a.oldvel
        robot_a.head = robot_a.oldhead
#     print(round(tcpa, 2), round(dcpa, 2))


def project(robot_a, angle1, angle2, centre):
    
    #  body frame to world frame velocity
    vela = robot_a.pos + polar2cart(robot_a.vel, robot_a.head)

    yintercept = centre[1] - (np.tan(angle1) * centre[0])
    # if slope is zero, projection is the x-coordinate itself
    if np.tan(angle1) == 0:
        newvela = np.array([vela[0], 0])
        return newvela
    else:
        xintercept = - yintercept / np.tan(angle1)

    # slope point to slope intercept
    # x_intercept = 3, y intercept = 1.5, slope = -0.5
    # line_prop = [3, 1.5, -0.5]
    line_prop = [xintercept, yintercept, np.tan(angle1)]

    # # slope - intercept form, poly1d(m, c)
    # line1 = np.poly1d([line_prop[2], line_prop[1]])

    # # plot the line
    # x_ax = np.linspace(-10, 10, 10)
    # y_ax = line1(x_ax)
    # plt.plot(x_ax, y_ax)

    # slope - intercept form, poly1d(m, c)
    line1 = np.poly1d([line_prop[2], line_prop[1]])

    # calculate vector passing through origin, y = mx
    vector_from_line = np.array([line_prop[0], -line_prop[1]])
    if (np.inner(vector_from_line, vector_from_line) > 0.1):
        # calculate projection matrix
        # projection matrix P**2 = P
        P = np.outer(vector_from_line, vector_from_line)/np.inner(vector_from_line, vector_from_line)
    else: 
        # back to body frame velocity
        newvela = vela - robot_a.pos
        return newvela
    
    projected_pt = P.dot(vela - np.array([0, line_prop[1]])) + np.array([0, line_prop[1]])
    newvela = projected_pt
    plt.plot(newvela[0], newvela[1], 'or', alpha=0.2)

    # back to body frame velocities
    newvela = newvela - robot_a.pos

    return newvela

def resolve(robot_a, robot_b):
    # build solution space diagram
    # find angle of b in world frame
    angleb = np.arctan2(robot_b.pos[1], robot_b.pos[0])
    # do atan dist here
    deltad = np.linalg.norm(robot_a.pos - robot_b.pos)
    angleb1 = angleb - np.arctan(RR/deltad)
    angleb2 = angleb + np.arctan(RR/deltad)
    # centre of collision cone

    centre = robot_a.pos + polar2cart(robot_b.vel, robot_b.head)
    # plot cc
    pt1 = centre
    pt2 = centre + np.array([(MAX_VEL * np.cos(angleb1)), (MAX_VEL * np.sin(angleb1))])
    pt3 = centre + np.array([(MAX_VEL * np.cos(angleb2)), (MAX_VEL * np.sin(angleb2))])
    plt.plot([pt2[0], pt1[0], pt3[0]], [pt2[1], pt1[1], pt3[1]], '-o', color='gray', alpha=0.2)
    
    oldvela = robot_a.pos + polar2cart(robot_a.vel, robot_a.head)
    plt.plot(oldvela[0], oldvela[1], 'og', alpha=0.2)
    
    newvela = project(robot_a, angleb1, angleb2, centre)   

    return newvela

robot_obj_list = []
def main():
    config_matplotlib()
    gray = "444444"
    fig = plt.figure()
    plt.grid(True, which='major')
    plt.grid(True, which='minor', color=gray, linestyle = '--', linewidth = 0.5)
    plt.minorticks_on()
    plt.axis('equal')
    plt.xlim(-15, 15)
    plt.ylim(-15, 15)

    robot_a = robot(np.array([7.0, 0.0]), 2, np.deg2rad(135))
    robot_b = robot(np.array([-7.0, 0.0]), 2, np.deg2rad(45))
    
    robot_obj_list.append(robot_a)
    robot_obj_list.append(robot_b)

    cnt = 0
    for i in range(35):
        # plt.cla()
        detect(robot_a, robot_b)
        # detect2(robot_obj_list)
        plt.plot(block = 'False')
        robot_a.move()
        robot_b.move()
        robot_a.draw(plt)
        robot_b.draw(plt)
        plt.plot()
        filename = 'logs/velobs%02d.png' % cnt
        cnt = cnt + 1
        plt.savefig(filename)
        plt.pause(0.1)
        # q = input('keypress to end')
        # if q != 'c':
        #     break
    
if __name__ == "__main__":
    main()