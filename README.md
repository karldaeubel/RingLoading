# RingLoading

This repository is dedicated to the famous Ring Loading Problem.

## Introduction

The Ring Loading Problem emerged in the 1990s to model an important special case of telecommunication networks (SONET rings) which gained attention from practitioners and theorists alike. Given an undirected cycle on n nodes together with non-negative demands between any pair of nodes, the Ring Loading Problem asks for an unsplittable routing of the demands such that the maximum cumulated demand on any edge is minimized. Let L be the value of such a solution. In the relaxed version of the problem, each demand can be split into two parts where the first part is routed clockwise while the second part is routed counter-clockwise. Denote with L* the maximum load of a minimum split routing solution. 

In a landmark paper, Schrijver, Seymour and Winkler \[SSW98\] showed that L <= L* + 3/2 D, where D is the maximum demand value. They also found (implicitly) an instance of the Ring Loading Problem with L = L* + 101/100 D. Recently, Skutella \[Sku16\] improved these bounds by showing  that L <= L* + 19/14 D, and there exists an instance with L = L* + 11/10 D. More recently, we showed  that L <= L* + 13/10 D \[Däu22\].

In this repository you find mixed integer linear programs that potentially produce new and improved lower bound instances. All you need is a strong computer and a solver for mixed integer linear programs.

## Instructions

If you are interested to find better lower bounds and contribute to research, use the given mixed integer programs provided in .lp format. The files rl*.lp model the base model as defined in \[Däu22\]. The files lp_reduced*.lp incorporate some reductions in order to improve the size of the mixed integer linear programs that should allow the instances to solve faster and require less momory.

If you want to solve or create your own mixed integer linear programs, you can use the source code provided in the generator folder. The code depends on gurobi and the boost program_options. It can be build with cmake.

### Results

Every objective value 
- larger than 1 is concidered a non-trivial instance
- larger than 1.1 is concidered an improved lower bound that is worth publishing
- larger than 1.3 is concidered a problem ;D (either the model or some theoretical result for upper bounds are wrong)

## Bibliography

\[SSW98\] A. Schrijver, P. Seymour, and P. Winkler. The ring loading problem. SIAM J. Discrete Math., 11(1):1–14, 1998.

\[Sku16\] M. Skutella. A note on the ring loading problem. SIAM J. Discrete Math., 30(1):327–342, 2016.

\[Däu22\] K. Däubel. An improved upper bound for the ring loading problem. SIAM J. Discrete Math., 36(2):867-887, 2022.
