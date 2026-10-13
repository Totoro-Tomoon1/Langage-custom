tb12 init c1 c2
c5
	tb11 nbBoucle ma 99
	tb11 i

	tb11 x
	tb11 y


	
	t5
c6

tb12 update c1 c2
c5
	i ma incr c1 i c2

	DrawPos c1 x y c2

	x ma x mb 1
	P x

	t2 c1 i f2 nbBoucle c2
	c5
		c3
	c6
c6

tb11 incr c1 tb11 _var c2
c5
	_var ma _var mb 1

	t5 _var
c6