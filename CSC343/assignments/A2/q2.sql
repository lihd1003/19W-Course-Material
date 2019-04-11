SET SEARCH_PATH to parlgov;

drop table if exists q2 cascade;

create table q2(
	countryName varchar(50),
    partyName varchar(100),
    partyFamily varchar(50),
    stateMarket real
);

SET SEARCH_PATH to parlgov;

drop view if exists result_id cascade;

create view result_id as
select cpc.party_id, cpc.country_id
from
	(select party_id, country_id, count(c.id)
	from cabinet_party as cp join cabinet as c on cp.cabinet_id = c.id
	where extract(year from start_date) >= 1996 and extract(year from start_date) <= 2016
	group by party_id, country_id) as cpc 
 join 
	(select country_id, count(id)
	from cabinet
	where extract(year from start_date) >= 1996 and extract(year from start_date) <= 2016
	group by country_id) as cc
on cpc.country_id = cc.country_id
where cpc.count = cc.count;

insert into q2
select country.name, party.name, family, state_market
from result_id as r 
	left join country on r.country_id = country.id
	left join party on r.party_id = party.id
	left join party_family on r.party_id = party_family.party_id
	left join party_position on r.party_id = party_position.party_id;

