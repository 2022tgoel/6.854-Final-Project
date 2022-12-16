```
sh time.sh | tee results.txt
```

results generated on MBP M1 Pro 2021 14in base model

---

results

Sigma notes: size = 200, sigma = 5. size = 30, sigma = 30. size = 75, sigma = 10. size = 150, sigma = 5.

size = 150 yields weird results for LCT. LCT takes 0.55s while dinic's takes 0.33s. however, in all the other sizes LCT is faster than dinic's.

size = 250, sigma = 3. dinic's takes 2.5s, LCT takes 3.3s??? wtf
size = 250, sigma = 4. dinic's takes 12.3s, LCT takes 2.8s???
both of these sigma values generates reasonably nice cuts.


-----

image 3: size = 250, sigma = 4. dinic's is 21.6s, LCT is 14.3s


---

baby: size = 200, sigma = 5. dinic's 9.4s, LCT 10s
