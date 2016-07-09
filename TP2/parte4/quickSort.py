def quickSort(n):
	s1=[]
	s2=[]
	if len(n)==0:
		return [];
	if len(n)==1:
		return n
	for i in range(len(n)):
		if n[i]<n[len(n)/2]:
			s1.append(n[i])
		elif n[i]>n[len(n)/2]:
			s2.append(n[i])
	return quickSort(s1) + [n[len(n)/2]] + quickSort(s2)
		

print quickSort([5,2,1,3,4])