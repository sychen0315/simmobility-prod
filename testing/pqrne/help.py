#!/usr/bin/python

# Copyright Singapore-MIT Alliance for Research and Technology

from PyQt4 import QtGui, QtCore

def create_manual():
    return Help_dialog()

class Help_dialog(QtGui.QDialog):
    def __init__(self):
        super(Help_dialog, self).__init__()
        self.setWindowTitle("pqrne manual")
        self.setMinimumWidth(600)

        label = QtGui.QLabel("""\
<h1 style="text-align: center;">pqrne</h1>
<h2>Overview</h2>
<p>pqrne is a graphical tool for editing road networks.  The lane markings and kerb lines are good candidates for the lane edges.  The editor provides facilities for the user to pick lane markings and kerb lines to be used as lane edges.</p>
<h2>Stringing lane markings to form lane edges</h2>
<p>Lane markings are not continuous along a road from one intersection to the next intersection; they change from one type to another along the way.  For example, type B changes to type C.  Whole-day bus lanes are marked by types L, A1, A2, A3, and A4.  Normal bus lanes are marked by S and S1 types.  Therefore you need to join lane markings to form a lane edge.</p>
<p>Set to <span style="color: red;">Snap to lines</span>.  Disable the <span style="color: red;">selectable</span> flags except for a minimum set of lane marking types or kerb line types.  This makes it easy for you to pick the correct lane markings.  Move the mouse over the target lane marking; the status bar will show its type (provided the type is selectable).  Right-click the lane marking to add it to the lane edge that you are trying to form.  When you have added the last section, move the mouse to an empty space and double-right-click.  A dialog box will pop up.</p>
<h2>Stringing kerb line points to form lane edges</h2>
<p>The kerb lines are usually very long and extend to the other roads.  For this reason, you shouldn't pick the entire kerb line; instead pick certain points of the kerb line (or kerb lines) to form a lane edge.  Set to <span style="color: red;">Snap to points</span> so that you only pick out points and not the lines.  When you have added the last point, move the mouse to an empty space and double-right-click.</p>
<h2>Generating the lane center lines</h2>
<p>After you have created the lane edges, you can use them to generate the lane center lines, which are the line in the middle of the lanes; the vehicles should stay on the center line unless they are changing lanes.</p>
<p>Right-click to select 2 lane edges.  Press the <span style="color: red;">Shift</span> key when you right-click to select the 2nd lane edge.  When you have 2 lane edges selected, press <span style="color: red;">g</span> to generate the center line.</p>
<h2>Marking out points for side-walks</h2>
<p>Because the side-walks are in the empty spaces, there are no points or lines to snap to.  You can mark out any point by moving the mouse to the target position and press the <span style="color: red;">Ctrl</span> and <span style="color: red;">Alt</span> keys when you right-click.  The <span style="color: red;">Snap to lines</span> or <span style="color: red;">Snap to points</span> mode is ignored when the 2 keys are pressed during a right-click; the mouse position will be used.</p>
<h2>Set the lane edge in the correct traffic flow direction</h2>
<p>When you join lane markings or kerb line points to form a lane edge, the lane edge polyline will be determined by the picking order.  Later when you create the lane center lines, the direction of the center line polyline will be identical to that of the lane edges.  Since the center line should be in the direction of the traffic flow, you must ensure the lane edges are also in the same direction.</p>
<p>It is important that you move the mouse to an empty space before you double-right-click to finish the construction of the current lane edge.  Otherwise, you will be picking up additonal lane marking or points to the lane edge.</p>
<h2>Cutting a group of lane edges and center lines</h2>
<p>A link, the portion of the road between 2 intersections, is usually divided into sections.  A section is the portion of the road between 2 nodes.  Therefore you should not create lane edges that cover the entire link, but only between the 2 nodes.  However this makes it more tedious because the lane markings and kerb line points are not aligned with the nodes.</p>
<p>The editor can bisect lane edges and center lines for you.  Select a lane edge that crosses the group of lane edges and center lines that you want to bisect.  This lane edge is known as the <span style="color: red;">cutting line</span>.  Most likely, you would need to create a temporary lane edge to serve as the cutting line.</p>
<p>For example, suppose there is a node 75m away from the intersection.  Create a lane edge at that point and parallel to the pedestrian crossing.  The cutting line starts at one side and ends on the other side of the road.  Make sure that the line cuts across the side-walks as well.</p>
<p>After you have selected the cutting line, press the <span style="color: red;">b</span> key.  After the bisection, delete the cutting line if necessary.</p>
<h2>Edit the data (section-id, type, etc) of a lane edge</h2>
<p>You can change a lane edge's data at any time.  Right-click to select the edge and press <span style="color: red;">e</span>; the dialog box will appear.</p>
<p>If you have a lane edge or center line selected, you can use this command to deselect it.  Press <span style="color: red;">e</span> and then click on the <span style="color: red;">Cancel</span> button.</p>
<h2>Delete a lane edge</h2>
<p>Right-click to select the edge and press <span style="color: red;">d</span>.</p>
<h2>Adjust the position of any point of a lane edge</h2>
<p>In many places, the lane markings do not begin at the pedestrian crossings or end at stop lines.  You can adjust any point of a lane edge or center line.  Click and drag a point and move it to the final position.</p>
<h2>Recommended procedure</h2>
<p>The following procedure can be used to edit the road network.
<ol>
<li>Join lane markings or kerb line points to form the lane edges within the link.  Start with the lane markings or kerb line points near the pedestrian crossing and move towards the stop line since vehicle traffic flows in that direction.</li>
<li>Use <span style="color: red;">Ctrl + Alt + right-click</span> to mark out some points to form the edges of the side walks; the order does not matter since pedestrian traffic flows both ways.</li>
<li>Adjust the end-points of the lane edges so that the edges for the vehicle lanes touch the pedestrian crossings and stop lines while the edges of the side walks meet the pedestrian crossings. 
<li>Select 2 lane edges; use <span style="color: red;">Shift + right-click</span> to select the 2nd edge.  Then press <span style="color: red;">g</span> to generate the center line.  Repeat this for each pair of lane edges in the link.</li>
<li>Press <span style="color: red;">n</span> to display the nodes.</li>
<li>Use <span style="color: red;">Ctrl + Alt + right-click</span> to mark out 2 points for the cutting line.  Make sure that the cutting line crosses the edges of the side walks.  Adjust the 2 points to set the position, extent, and orientation of the cutting line; it should be as close to perpendicular to the lane edges and center lines as possible.</li>
<li>Press <span style="color: red;">b</span> to split the lane edges and center lines at the cutting line.  After the cut, do not adjust the center lines anymore since each center line begins where the previous center line ends.  If you make any adjustment, it is possible that the vehicle may jump from one center line to the next center line during the simulation.</li>
<li>Select all the lane edges and center lines on the left of the cutting line; use <span style="color: red;">Shift + right-click</span> to select the 2nd and subsequent edges and lines.  Press <span style="color: red;">e</span> and enter the <span style="color: red;">section-id</span> for these edges and lines.  Do the same for the edges and lines on the right of the cutting line.</li>
<li>Select the cutting line and press <span style="color: red;">d</span> to remove it.</li>
</ol>
</p>
<p>You can use the cutting facility in step 3 above.  Adjust the ends of the lane edges so that the edges extend slightly beyond the pedestrian crossings or stop lines.  Then select the crossing or stop line as the cutting line.  After the cut, remove the short edges.</p> 
<h2>Misc</h2>
<p>Turn the mouse-wheel to zoom in and out.  The <span style="color: red;">-</span> key also zooms out while the <span style="color: red;">+</span> and <span style="color: red;">=</span> keys zoom in.</p>
<p>Click and drag to scroll to the left or right, up or down.</p>
<p>You can rotate the view clockwise with the capital <span style="color: red;">C</span> key and anti-clockwise with the capital <span style="color: red;">A</span> key.  To revert to the original orientation, press the capital <span style="color: red;">O</span> key.</p>
<p>Press <span style="color: red;">H</span> to toggle the visibility of the points of the lane edge and center line polylines.</p>
<p>To save the road network without quitting, press <span style="color: red;">S</span>.</p>
<p>To toggle the visibility of the Node points, press <span style="color: red;">N</span>.</p>
""")
        label.setWordWrap(True)
        scroll_area = QtGui.QScrollArea()
        scroll_area.setWidget(label)

        layout = QtGui.QVBoxLayout()
        layout.addWidget(scroll_area)
        button_box = QtGui.QDialogButtonBox(QtGui.QDialogButtonBox.Ok, QtCore.Qt.Horizontal)
        button_box.setCenterButtons(True)
        layout.addWidget(button_box)
        self.setLayout(layout)

        self.connect(button_box, QtCore.SIGNAL("accepted()"), self, QtCore.SLOT("accept()"))
