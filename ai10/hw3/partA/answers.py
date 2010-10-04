# coding: utf-8
# exercise 1
# test answers, don't comment on them
ex1=[
	0, # that it does not differentiate between actions and states
	1, # when solving a POMDP one typically solves an MDP and therefore use the methods developed for MDPs.
	2, # regression planners (1961-1998), because they perform only relevant actions
	0, # the belief state in POMDP's is fully observable and continuous
	2, # you might need a discount factor even if you have a terminal state
	1, # you can train your HMM using Baum-Welch once you decide manually the number of states in your model
	0] # none of the other two sentences

# exercise 2

# Calculating probabilities:
# A', notation for "not A": P(A') = 1 - P(A)
#
# P(UN) = 0.990
# P(Militia) = 0.796 * P(UN)
#            + 0.417 * P(UN')
# P(Tabloids) = 0.802 * P(UN)
#             + 0.156 * P(UN')
# P(XWIN) = 0.899 * P(Militia) * P(Tabloids)
#         + 0.598 * P(Militia) * P(Tabloids')
#         + 0.424 * P(Militia') * P(Tabloids)
#         + 0.179 * P(Militia') * P(Tabloids')

# Ex A: Probablility that X wins (given nothing).
# P(Militia) = 0.796 * 0.990 + 0.417 * (1-0.990) = 0.79221
# P(Tabloids) = 0.802 * 0.990 + 0.156 * (1-0.990) = 0.79554
# P(XWIN) = 0.899 * 0.79221 * 0.79554 + 0.598 * 0.79221 * (1-0.79554)
#         + 0.424 * (1-0.79221) * 0.79554 + 0.179 * (1-0.79221) * (1-0.79554)
#         = 0.7411422229056 (not rounded)
ex2a = [0.7411422229056]

# Ex B:
#
# Calculate P(XWIN|Militia):
# P(UN|Militia) = P(UN) = 0.990
# P(Militia) = 1
# P(Tabloids|Militia) = {samma som Ex A} = 0.79556
#
# P(XWIN|Militia) = 0.899 * P(Tabloids)
#                 + 0.598 * P(Tabloids')
#                 = 0.83746356
#
# The economical difference between X winning and losing is (win - lose),
# 1 000 000 - (-500 000) = 1 500 000 (1.5 million)
# A win is therefore worth 1 500 000 more than a loss
#
# Difference in probability (our purchased statistical advantage) is:
#
# DIFF = P(XWIN|Militia) - P(XWIN) = 0.83746356 - 0.7411422229056 = 0.0963213370944
# Militia's vote is therefore worth DIFF * 1 500 000 to the company, that is: 144482.0056416
#
# (One could also argue that the company'd be willing to pay nothing, because it is fairly immoral.)
# Personally, I'd answer ex2b = [0].
ex2b = [ 144482.0056416 ]

# Ex C: Find P(UN'|XWIN'), P(Militia'|XWIN') and P(Tabloid'|XWIN')
#
# P(XWIN|UN):
# P(Militia|UN) = 0.796
# P(Tabloids|UN) = 0.802
# P(XWIN|UN) = 0.899 * P(Militia) * P(Tabloids)
#            + 0.598 * P(Militia) * P(Tabloids')
#            + 0.424 * P(Militia') * P(Tabloids)
#            + 0.179 * P(Militia') * P(Tabloids')
#
#            = 0.899 * 0.796 * 0.802
#            + 0.598 * 0.796 * (1-0.802)
#            + 0.424 * (1-0.796) * 0.802
#            + 0.179 * (1-0.796) * (1-0.802)
#
#            = 0.744763952
#
# Find P(UN'|XWIN'):
# P(XWIN'|UN) = P(XWIN' ∩ UN) / P(UN)
# P(XWIN' ∩ UN) = P(XWIN'|UN) * P(UN)
#
# P(UN|XWIN') = P(XWIN' ∩ UN) / P(XWIN')
#             = P(XWIN'|UN) * P(UN) / P(XWIN')
#
#
# P(UN'|XWIN') = 1 - P(XWIN'|UN) * P(UN) / P(XWIN')    # This is what we want.
#
# P(UN) given = 0.990
# P(XWIN) known. {Ex A}
# P(XWIN') = 1 - P(XWIN) = 0.2588577770944
#
# P(XWIN|UN) known.
# P(XWIN'|UN) = 1 - P(XWIN|UN) = 0.255236048
#
# Probability that UN didn't vote for X: P(UN'|XWIN')
#   = 1 - 0.255236048 * 0.990 / 0.2588577770944
#   ≈ 0.02385128097638124690
# 
#
# P(XWIN|Militia):
# P(XWIN|Militia) = {Ex B} = 0.83746356
#
# P(XWIN'|Militia) = 1 - P(XWIN|Militia) = 0.16253644
#
# P(Militia'|XWIN') = { Like P(UN'|XWIN') } = 1 - P(XWIN'|Militia) * P(Militia) / P(XWIN')
# Probability that Militia didn't vote for X: P(Militia'|XWIN')
#   = 1 - 0.16253644 * 0.79221 / 0.2588577770944 
#   ≈ 0.50257243735256662547
#
#
# P(XWIN|Tabloids):
#
# P(Militia|Tabloids) = P(Militia) = {Ex A} = 0.79221
# P(Tabloids) = {Ex A} = 0.79554
#
# P(XWIN|Tabloids) = 0.899 * P(Militia)
#                + 0.424 * P(Militia')
#                = 0.899 * 0.79221 + 0.424 * (1-0.79221)
#                = 0.80029975
#
# P(XWIN'|Tabloids) = 1 - P(XWIN|Tabloids) = 0.19970025
#
# P(Tabloids'|XWIN') = { Like P(UN'|XWIN') } = 1 - P(XWIN'|Tabloids) * P(Tabloids) / P(XWIN')
# Probability that Tabloids didn't vote for X: P(Tabloids'|XWIN')
#   = 1 - 0.19970025 * 0.79554 / 0.2588577770944
#   ≈ 0.38626708971906371092
ex2c = [
	0.02385128097638124690, # Probability that UN didn't vote for X, given that X lost. REASONABLE? DOUBLE-CHECK IT!
	0.50257243735256662547, # Probability that Militia didn't vote for X, given that X lost.
	0.38626708971906371092] # Probability that Tabloids didn't vote for X, given that X lost. 

# exercise three
# computed with ex3.py, attached
# discount = 0.9
# [0.71000000000000008, 1.0, -1.0, 0.62094761180000013, 0.65510000000000013]
ex3a={1:0.710,3:0.6209,4:0.6551}
# discount = 0.1
# [0.070000000000000021, 1.0, -1.0, -0.0037373737999999981, 0.0063000000000000052]
ex3b={1:0.070,3:-0.00373737,4:0.00630}
