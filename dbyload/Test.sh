

Pause ()
{
	echo "Press [ENTER] to continue"
	read x
	echo ""
}

echo "*** Usage message ***"
dbyload
Pause

echo "*** head Test.csv ***"
head Test.csv
Pause

echo "*** Specification file Test.spec ***"
cat Test.spec
Pause

echo "*** insert script created with dbyload -n ...  ***"
dbyload -n nutrition food Test.csv Test.spec -s '|'
Pause

echo "*** update script created with dbyload -u ...  ***"
dbyload -u nutrition food Test.csv Test.spec -s '|'

