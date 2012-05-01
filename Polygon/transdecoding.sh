for i in `find ./ -type f -name "*"`;
do iconv $i -f gbk -t utf8 -o ${i}.tmp && mv ${i}.tmp $i;
done
