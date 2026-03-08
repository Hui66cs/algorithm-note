
- **Name:朱恒辉**
- **Student ID：12412412**
---

## Brief Description
- preprocessing: sort the preference list for every students. For each college, create an inverse mapping to check the evaluation score for a specific student in O(1)    
- maintain a queue for free student(all students in it at the begining)
- when the queue is not empty, pop a student and apply to the favourite college for him which he evaluate it with positive score and the college hasn't rejected him yet and college's evaluation score for him is positive. If the college isn't full , they wull be matched temporarily. If the college is full and he is better than the worst student who has been temporarily accepted, the college should free the worst student and accept the current student temporarily. 

---

## Complexity Analysis

### Time Complexity 

1. for sorting the students' preference list, it takes $O(N \times M \times logM)$ 
2. for the inverse mapping of colleges' preference list(in this exercise it doesn't need extra operations) takes $O(M \times N)$
3. - During the loop, the worst case is each students apply to every colleges, so it need $O(N \times M)$ round.
   - In each round, the worst case needs to check the worst student accepted, so we maintains a min-heap, which takes $O(log (cap_{max}))$, where $cap_{max}$ is the capcity of the school which has largest capacity, and $cap_{max}=O(N)$.
   - Other operations are O(1). 
   - So the loop totally takes $O(N \times M \times log(cap_{max}))$ 
4. So in total, the time complexity is $(N \times M \times (logM+log(cap_{max})))=O(N \times M \times (logM+logN))$

### Space Complexity
1. the students' preference list and colleges' preference list both takes $O(N \times M)$
2. the free_student queue takes $O(N)$
3. - recording students'match needs $O(N)$
   - recording colleges'match needs $O(M \times q_{max})=O(M \times N)$
4. each min-heap used to search for the worst accepted student needs $O(q_{max})$, so M min-heaps need $O(M \times q_{max})=O(M \times N)$
5. so in total, the space complexity is $O(M \times N)$

---

## Stable Matching

### Definition
1.  No student is assigned to a college whose score he evaluate for it <0 , and no college accepts a student whose score it evaluate for him <0
2. There doesn't exist an unstable pair(s,c) such that 
   - student s prefers college c to the current college which accepted him or s is unmatched and his score to c is positive
   - the college c prefers student s to worst student it accepted or c is not full and its score to s is positive

### Proof
When students choose the college, it first needs to check the score>0 for both preference list, so definition 1 is obviously satisfied.
Now I prove the definition2(the unstable pair).
- Assume there exist an unstable pair(s1,c1). 
- Let s1 now is accepted by c2 which leads the preference list for s1 needs to satisfy s1: c1>c2, or s1 is unmatched
- Let c1 now accepted s2,s3,s4,... the preference list for c1 needs to satisfy c1: s1>min{s2,s3,s4,...}
- 1. if s1 has not applied to c1 and s1 is matched with c2, that means he applied to c2 first and be accepted until the end, so in s1's preference list: s1: c2>c1, it forms a contradiction
  2. if s1 has not applied to c1 and s1 is unmatched, it only happens when the score s1 gives to c1 is negative, so (s1,c1) is stable, it forms a contradiction 
  3. if s1 has applied to c1, and c1 is full, due to the accepted student will be monotonically strictly better, c1 prefers its current students to s1, so (s1,c1) is stable, it forms a contradiction
  4. if s1 has applied to c1, and c1 is not full, it only happens when the score c1 gives to s1 is negative, so it also forms a contradiction
- Due to the contradiction, (s1,c1) should be a stable pair. So there doesn't exist an unstable pair

## Student-Optimal

### Definition
1. c is s's valid college means there exists a stable matching that s is assigned to c
2. a matching is student-optimal if every student is assigned to their most preferred valid college

### Proof
- Assume a stable matching M* is not student-optimal. Let s1 be the first student who isn't assigned to his most preferred valid college. c1 is his most preferred valid college that reject him.
- since c1 is s1's valid partner, there exists a stable matching M, that s1 is assigned to c1. So c1 to s1's score must be positive
- since students apply to college in decreasing preferred order, so in M* s1 must apply to c1 but rejected. Due to c1 to s1's score is positive, c1 must be full. So c1 prefers accepted students to s1, let s2 be the one who isn't chosen by c1 in M(since M's capacity is a const, it accept c1, due to pigeonhole principle at least one in M* will be rejected). We have c1:s2>s1
- Notice that in M*, s1 is the first student who isn't assigned to his most preferred valid college. And c1 can reject s1, that means c1 is full, s2 is assigned to c1, and c1 is s2's most preferred college. So we have s2:c1> other colleges
- with c1:s2>s1 , s2:c1> other, and in M (s1,c1) is a pair. We can conclude that it is an unstable pair, this contradicts to assuming M is a stable matching. So it's student-optimal
