VERSION=`cat VERSION`
rm ../formast-${VERSION}-src.zip
git clean -xfd
find . -type f -not -wholename "*.git*" -and -not -name ".project" | xargs zip ../formast-${VERSION}-src.zip
