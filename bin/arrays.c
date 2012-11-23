//@ #include "arrays.gh"

/*@

lemma void ints_split(int *array, int offset)
    requires ints(array, ?N, ?as) &*& 0 <= offset &*& offset <= N;
    ensures ints(array, offset, take(offset, as)) &*& ints(array + offset, N - offset, drop(offset, as));
{
  open ints(array, N, as);
  if (offset == 0) {
  } else {
      ints_split(array + 1, offset - 1);
  }
}

lemma void ints_join(int *a)
    requires ints(a, ?M, ?as) &*& ints(a + M, ?N, ?bs);
    ensures ints(a, M + N, append(as, bs));
{
    open ints(a, M, as);
    if (M == 0) {
    } else {
        ints_join(a + 1);
    }
}

lemma void ints_unseparate_same(int *array, list<int> xs)
    requires ints(array, ?M, take(M, xs)) &*& integer(array + M, head(drop(M, xs))) &*& ints(array + M + 1, ?N, tail(drop(M, xs))) &*& length(xs) == M + N + 1;
    ensures ints(array, M + N + 1, xs) &*& head(drop(M, xs)) == nth(M, xs);
{
    open ints(array, _, _);
    assert xs == cons(_, ?xs0);
    if (M != 0) {
        ints_unseparate_same(array + 1, xs0);
    }
}

lemma void ints_unseparate(int *array, int i, list<int> xs)
    requires ints(array, i, take(i, xs)) &*& integer(array + i, ?y) &*& ints(array + i + 1, length(xs) - i - 1, tail(drop(i, xs)));
    ensures ints(array, length(xs), update(i, y, xs));
{
    open ints(array, _, _);
    assert xs == cons(_, ?xs0);
    if (i != 0) {
        ints_unseparate(array + 1, i - 1, xs0);
    }
}

@*/
