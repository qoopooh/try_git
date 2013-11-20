from numpy import arange

a = arange(15).reshape(3, 5)

print 'array a', a
print "array a's shape", a.shape
print "array a's ndim", a.ndim
print "elements's type name", a.dtype.name
print "a's type", type(a)
print "a's size", a.size

