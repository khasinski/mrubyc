# Benchmark: callinfo push/pop via deep method calls
# This stresses mrbc_push_callinfo / mrbc_pop_callinfo

def noop
  nil
end

def call1
  noop
end

def call2
  call1
end

def call3
  call2
end

i = 0
while i < 100000
  call3
  i += 1
end
