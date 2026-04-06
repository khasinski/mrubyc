# Mixed workload: method calls, recursion, array ops
class Node
  def initialize(val, left, right)
    @val = val
    @left = left
    @right = right
  end

  def sum
    s = @val
    s += @left.sum if @left
    s += @right.sum if @right
    s
  end
end

def build_tree(depth)
  if depth == 0
    Node.new(1, nil, nil)
  else
    Node.new(depth, build_tree(depth - 1), build_tree(depth - 1))
  end
end

i = 0
while i < 500
  tree = build_tree(4)
  tree.sum
  i += 1
end
