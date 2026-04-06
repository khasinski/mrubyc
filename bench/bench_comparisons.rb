# Array and comparison heavy workload (fewer method calls)
a = [5, 3, 8, 1, 9, 2, 7, 4, 6, 0]
j = 0
while j < 5000
  # bubble sort
  n = a.length
  i = 0
  while i < n - 1
    k = 0
    while k < n - 1 - i
      if a[k] > a[k + 1]
        tmp = a[k]
        a[k] = a[k + 1]
        a[k + 1] = tmp
      end
      k += 1
    end
    i += 1
  end
  j += 1
end
