set search_path to parlgov;

drop table if exists q5 cascade;

create table q5(
	countryName varchar(50),
    year int,
    participationRatio real
);

drop view if exists ratio_view cascade;

create view ratio_view as
select country_id, year, avg(ratio) as ratio_avg
from
(select country_id, extract(year from e_date) as year, votes_cast:: float / electorate::float as ratio
from election
where extract(year from e_date) >= 2001 and extract(year from e_date) <= 2016
	and votes_cast is not null and electorate is not null) as temp
group by country_id, year;

insert into q5
select name, year, ratio_avg
from ratio_view join country on country_id = id
where country_id not in
(select distinct r1.country_id
from ratio_view r1, ratio_view r2
where r1.country_id = r2.country_id and
	r1.year < r2.year and 
	r1.ratio_avg > r2.ratio_avg);


