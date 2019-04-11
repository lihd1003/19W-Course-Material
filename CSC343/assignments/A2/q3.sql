SET SEARCH_PATH to parlgov;

drop table if exists q3 cascade;

create table q3(
       countryName varchar(100),
       partyName varchar(100),
       partyFamily varchar(100),
       wonElections integer,
       mostRecentlyWonElectionId integer,
       mostRecentlyWonElectionYear integer
);

drop view if exists election_win cascade;

create view election_win as
select party_id, er.election_id, e_date
from (select election_id, max(votes) as max_votes
	 from election_result group by election_id
	) as max_e
join election_result as er on max_votes = votes and max_e.election_id = er.election_id
join election on election.id = er.election_id;


drop view if exists party_win cascade;

create view party_win as
select party_id, country_id, count(election_win.election_id) as wonElections
from election_win
	join election on election.id = election_win.election_id
group by party_id, country_id;

drop view if exists country_avg cascade;

create view country_avg as 
select numParty.country_id, (numEleciton.count::real / numParty.count:: real) as avg_win
from 
	(select country_id, count(id) as count
	from party
	group by country_id
	) as numParty
join 
	(select country_id, count(id) as count
	from election
	group by country_id
	) as numEleciton
on numParty.country_id = numEleciton.country_id;

drop view if exists raw cascade;

create view raw as
select party_id, party_win.country_id, wonElections
from party_win join country_avg on party_win.country_id = country_avg.country_id
where wonElections > 3 * avg_win;

drop view if exists recentWin cascade;

create view recentWin as
select recent.party_id, election_id, extract(year from recent_date) as year
from election_win join
	(select party_id, max(e_date) as recent_date
	 from election_win 
	 group by party_id) as recent 
on e_date = recent_date and recent.party_id = election_win.party_id;

insert into q3 
select country.name, party.name, party_family.family, wonElections, recentWin.election_id, recentWin.year
from raw 
	left join country on country.id = raw.country_id
	left join party on party.id = raw.party_id
	left join party_family on party_family.party_id = raw.party_id
	left join recentWin on recentWin.party_id = raw.party_id;



	
