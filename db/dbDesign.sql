-- 
-- path 
-- 
-- 
create table files (abPath varchar(300), fName varchar(100), extension char(8), fType char(1), 
    lastUpdateTime date, primary key (abPath, fName));
-- example for test
insert into files values("Z:\", "ABC.doc", "doc", "F", "2019-01-02-12-24-32");
-- insert into files values("Z:\", "DEF.doc", "doc", "F", "2019-01-02--12-24:32");

-- 音乐 - 1, 图片 - 2, 视频 - 3, 文档 - 4
create table MediaType (extension char(8) primary key, typeCode int);
insert into MediaType values ('mp3', 1), ('wav', 1);
insert into MediaType values ('jpg', 2), ('png', 2), ('gif', 2), ('jpeg', 2);
insert into MediaType values ('mp4', 3), ('mkv', 3);
insert into MediaType values ('doc', 4), ('docx', 4), ('pdf', 4);