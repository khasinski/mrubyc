# Mixed benchmark - combines arrays, hashes, strings, and computation

# Array building and manipulation
10.times do
  a = []
  i = 0
  while i < 300
    a << i * 2
    i += 1
  end
  a.length
end

# Hash operations
h = {}
i = 0
while i < 500
  h[i] = i * 3
  i += 1
end

# String building
s = ""
i = 0
while i < 500
  s += i.to_s
  i += 1
end

# Computation
total = 0
i = 0
while i < 3000
  total += i * 2 + 1
  i += 1
end

# Array of hashes
data = []
i = 0
while i < 100
  data << {value: i, label: i.to_s}
  i += 1
end
