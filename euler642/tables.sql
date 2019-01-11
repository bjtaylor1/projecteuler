drop table calcs
drop table newcalcs

create table calcs
(
	N bigint not null,
	i int not null,
	h bigint null
)

create table newcalcs
(
	N bigint not null,
	i int not null
)

create unique clustered index idx_calcs_N_i on calcs(N, i)
create unique clustered index idx_newcalcs_N_i on newcalcs(N, i)

create table primes
(
	i int not null,
	p int not null
)

create unique clustered index idx_primes_i on primes(i)

select max(i) from primes

select count(*) from calcs
select count(*) from newcalcs