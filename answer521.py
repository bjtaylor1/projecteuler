def min_factor_sum(N):
  """
  10 **  7: 3203714961609
  10 **  8: 279218813374515
  10 **  9: 24739731010688477
  10 ** 10: 2220827932427240957
  10 ** 11: 201467219561892846337
  10 ** 12: 18435592284459044389811
  10 ** 13: 1699246543196666002725979
  10 ** 14: 157589263416765879793706013
  """
  def f(n):
    return n * (n + 1) // 2 - 1

  v = int(N ** 0.5)
  s_cnt = [i - 1 for i in range(v + 1)]
  s_sum = [f(i) for i in range(v + 1)]
  l_cnt = [N // i - 1 if i else 0 for i in range(v + 1)]
  l_sum = [f(N // i) if i else 0 for i in range(v + 1)]
  
  ret = 0
  for p in range(2, v + 1):
    if s_cnt[p] == s_cnt[p - 1]:
      continue
    p_cnt = s_cnt[p - 1]
    p_sum = s_sum[p - 1]
    q = p * p

    ret += p * (l_cnt[p] - p_cnt)
    
    end = min(v, N // q)
    for i in range(1, end + 1):
      d = i * p
      if d <= v:
        l_cnt[i] -= l_cnt[d] - p_cnt
        l_sum[i] -= (l_sum[d] - p_sum) * p
      else:
        t = N // d
        l_cnt[i] -= s_cnt[t] - p_cnt
        l_sum[i] -= (s_sum[t] - p_sum) * p

    for i in range(v, q - 1, -1):
      t = i // p
      s_cnt[i] -= s_cnt[t] - p_cnt
      s_sum[i] -= (s_sum[t] - p_sum) * p

  return l_sum[1] + ret

print(min_factor_sum(10 ** 2))