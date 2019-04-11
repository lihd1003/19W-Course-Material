set search_path to parlgov;

drop table if exists q6 cascade;

create table q6(
	countryName varchar(50),
	r0_2 integer, 
	r2_4 integer,
	r4_6 integer,
	r6_8 integer,
	r8_10 integer
);

drop view if exists party_lr cascade;

create view party_lr as
select country.name, left_right
from party_position join party on party_id = id join country on country_id = country.id
where left_right <> 0;

insert into q6
select *
from 
	(select name, count(left_right) as r0_2
	from party_lr
	where left_right < 2
	group by name) as q02
natural join
	(select name, count(left_right) as r2_4
	from party_lr
	where left_right < 4 and left_right >= 2
	group by name) as q24
natural join
	(select name, count(left_right) as r4_6
	from party_lr
	where left_right < 6 and left_right >= 4
	group by name) as q46
natural join
	(select name, count(left_right) as r6_8
	from party_lr
	where left_right < 8 and left_right >= 6
	group by name) as q68
natural join
	(select name, count(left_right) as r8_10
	from party_lr
	where left_right <= 10 and left_right >= 8
	group by name) as q810