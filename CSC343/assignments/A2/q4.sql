SET SEARCH_PATH to parlgov;

drop table if exists q4 cascade;

create table q4(
year int,
countryName varchar(50),
voteRange varchar(20),
partyName varchar(100)
);

insert into q4
select t.year as year, country.name as countryName, 
	(case when t.percentage <= 5 then '(0-5]'
		 when t.percentage > 5 and t.percentage <= 10 then '(5-10]'
		 when t.percentage > 10 and t.percentage <= 20 then '(10-20]'
		 when t.percentage > 20 and t.percentage <= 30 then '(20-30]'
		 when t.percentage > 30 and t.percentage <= 40 then '(30-40]'
		 else '(40-100]'
	end) as voteRange, party.name_short as partyName
	
from 
	(select temp.year, temp.country_id, temp.party_id, avg(temp.percentage) as percentage
	from 
		(select extract(year from e_date) as year, election_id, 
	 	country_id, party_id, (sum(votes) * 100 / sum(votes_valid))::float as percentage
		from election as e join election_result as r on e.id=r.election_id
		where extract(year from e_date) >= 1996 and extract(year from e_date) <= 2016
			and votes_valid is not null 
			and votes is not null
		group by extract(year from e_date), country_id, party_id, election_id
		) as temp
	group by temp.year, temp.country_id, temp.party_id
	) as t 
left join country on country.id = t.country_id 
left join party on party.id = t.party_id;

