set search_path to parlgov;

select *
from q4
order by year desc, countryName desc, voteRange desc, partyName desc