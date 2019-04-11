SET SEARCH_PATH to parlgov;


drop table if exists q1 cascade;
drop view if exists pair_count cascade;
drop view if exists country_count cascade;

-- count all pairs of allies in election of a country
create view pair_count as
select alliedPartyId1, alliedPartyId2, country_id, count(pair.election_id)
from election as e join 
(
	select e1.party_id as alliedPartyId1, e2.party_id as alliedPartyId2, e1.election_id
 	from election_result e1, election_result e2
 	where e1.election_id = e2.election_id 
 		and e1.party_id < e2.party_id and 
			 (e1.alliance_id = e2.alliance_id or 
			  e1.alliance_id = e2.id)	 
) as pair on e.id = pair.election_id
group by alliedPartyId1, alliedPartyId2, country_id; 

-- count the number of elections in each country
create view country_count as 
select country_id, count(election.id)
from election
group by country_id;


create table q1(countryId integer, 
				alliedPartyId1 integer,
				alliedPartyId2 integer);
insert into q1
select pair_count.country_id as countryID, alliedPartyId1, alliedPartyId2
from pair_count join country_count on pair_count.country_id = country_count.country_id
where pair_count.count >= (country_count.count * 0.3);