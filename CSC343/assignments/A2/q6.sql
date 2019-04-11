SET SEARCH_PATH TO parlgov;
drop table if exists q4 cascade;

-- You must not change this table definition.


CREATE TABLE q4(
        countryName VARCHAR(50),
        r0_2 INT,
        r2_4 INT,
        r4_6 INT,
        r6_8 INT,
        r8_10 INT
);

-- You may find it convenient to do this for each of the views
-- that define your intermediate steps.  (But give them better names!)
DROP VIEW IF EXISTS PartyPos CASCADE;

CREATE VIEW PartyPos AS 
SELECT p.id, p.country_id, pp.left_right
FROM Party p JOIN Party_position pp ON p.id = pp.party_id
WHERE pp.left_right IS NOT NULL;  

DROP VIEW IF EXISTS Result0to2 CASCADE;

CREATE VIEW Result0to2 AS
SELECT PartyPos.country_id, count(*) as r0_2
FROM PartyPos
WHERE left_right >= 0 AND left_right < 2
GROUP BY PartyPos.country_id;

DROP VIEW IF EXISTS Result0to2Com CASCADE;

CREATE VIEW Result0to2Com AS
(SELECT id, 0 as r0_2
FROM country
WHERE id not in (
        SELECT country_id
        FROM Result0to2
)) UNION (SELECT * FROM Result0to2);


DROP VIEW IF EXISTS Result2to4 CASCADE;

CREATE VIEW Result2to4 AS
SELECT PartyPos.country_id, count(*) as r2_4
FROM PartyPos
WHERE left_right >= 2 AND left_right < 4
GROUP BY PartyPos.country_id;

DROP VIEW IF EXISTS Result2to4Com CASCADE;

CREATE VIEW Result2to4Com AS
(SELECT id, 0 as r2_4
FROM country
WHERE id not in (
        SELECT country_id
        FROM Result2to4
)) UNION (SELECT * FROM Result2to4);

DROP VIEW IF EXISTS Result4to6 CASCADE;

CREATE VIEW Result4to6 AS
SELECT PartyPos.country_id, count(*) as r4_6
FROM PartyPos 
WHERE left_right >= 4 AND left_right < 6
GROUP BY PartyPos.country_id;

DROP VIEW IF EXISTS Result4to6Com CASCADE;

CREATE VIEW Result4to6Com AS
(SELECT id, 0 as r4_6
FROM country
WHERE id not in (
        SELECT country_id
        FROM Result4to6
)) UNION (SELECT * FROM Result4to6);

DROP VIEW IF EXISTS Result6to8 CASCADE;

CREATE VIEW Result6to8 AS
SELECT PartyPos.country_id, count(*) as r6_8
FROM PartyPos
WHERE left_right >= 6 AND left_right < 8
GROUP BY PartyPos.country_id;

DROP VIEW IF EXISTS Result6to8Com CASCADE;

CREATE VIEW Result6to8Com AS
(SELECT id, 0 as r6_8
FROM country
WHERE id not in (
        SELECT country_id
        FROM Result6to8
)) UNION (SELECT * FROM Result6to8);

DROP VIEW IF EXISTS Result8to10 CASCADE;

CREATE VIEW Result8to10 AS
SELECT PartyPos.country_id, count(*) as r8_10
FROM PartyPos
WHERE left_right >= 8 AND left_right < 10
GROUP BY PartyPos.country_id;

DROP VIEW IF EXISTS Result8to10Com CASCADE;

CREATE VIEW Result8to10Com AS
(SELECT id, 0 as r8_10
FROM country
WHERE id not in (
        SELECT country_id
        FROM Result8to10
)) UNION (SELECT * FROM Result8to10);

DROP VIEW IF EXISTS Result CASCADE;

CREATE VIEW Result AS
SELECT r1.id AS country_id , r0_2, r2_4, r4_6, r6_8, r8_10
FROM Result0to2Com r1, Result2to4Com r2, Result4to6Com r3, Result6to8Com r4, Result8to10Com r5
WHERE r1.id = r2.id 
AND r2.id = r3.id
AND r3.id = r4.id
AND r4.id = r5.id;

DROP VIEW IF EXISTS FinalResult CASCADE;

CREATE VIEW FinalResult AS
SELECT c.name AS countryName, r0_2, r2_4, r4_6, r6_8, r8_10
FROM Result as r JOIN Country as c ON r.country_id = c.id;

-- the answer to the query 
INSERT INTO q4 (SELECT * FROM FinalResult);