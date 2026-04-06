# Array benchmark - tests array growth via push
# This is the primary benchmark for the growth factor optimization

# Build a large array via push (triggers many resizes)
a = []
i = 0
while i < 500
  a << i
  i += 1
end

# Do it multiple times to amplify the effect
20.times do
  b = []
  j = 0
  while j < 500
    b << j
    j += 1
  end
end

# Array concatenation
c = [1,2,3]
15.times do
  c = c + [4,5,6,7,8,9,10]
end

# Nested array building
10.times do
  d = []
  k = 0
  while k < 200
    d << [k, k+1, k+2]
    k += 1
  end
end
