import math

if __name__ == "__main__":
    lowest_x = lowest_y = lowest_z = math.inf
    with open('../required_files/Plane.obj', 'r') as f:
        for each_line in f.readlines():
            try:
                elements = each_line.split(' ')
                if len(elements) == 4 and elements[0] == 'v':
                    # It's a vertex
                    x_coordinate = float(elements[1])
                    y_coordinate = float(elements[2])
                    z_coordinate = float(elements[3])
                    if x_coordinate < lowest_x:
                        lowest_x = x_coordinate
                    if y_coordinate < lowest_y:
                        lowest_y = y_coordinate
                    if z_coordinate < lowest_z:
                        lowest_z = z_coordinate
            except:
                continue
    print('Lowest x coordinate: ', lowest_x)
    print('Lowest y coordinate: ', lowest_y)
    print('Lowest z coordinate: ', lowest_z)
