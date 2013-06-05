from geo.position import Location
from geo.helper import assert_non_null
from geo.helper import dict_to_lower

class RoadNetwork:
  '''The primary container class for OSM road networks. (See: simmob.py)
     Note that key/value properties are reduced to lowercase for both keys
     and values.
  '''

  def __init__(self):
    self.bounds = []   #[Location,Location], min. point, max. pt.
    self.nodes = {}    #origId => Node
    self.ways = {}     #origId => Way


class Node:
  def __init__(self, nodeId, lat, lng, props):
    assert_non_null("Null params", nodeId, lat, lng, props)
    self.nodeId = str(nodeId)
    self.loc = Location(float(lat), float(lng))
    self.props = dict_to_lower(props)


class Way:
  '''Ways are somewhat different from Links: they don't have 
     "from" and "to" Nodes, but rather feature an ordered sequence of Nodes.
  '''

  def __init__(self, wayId, nodes, props):
    assert_non_null("Null params", wayId, nodes, props)
    if len(nodes)<2:
      raise Exception('Way cannot be made with less than 2 Nodes.')

    self.wayId = str(wayId)
    self.nodes = nodes   #[Node]
    self.props = dict_to_lower(props)

