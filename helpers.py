from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    similarities = set()
    a_lines = a.splitlines()
    for line in a_lines:
        if line in b:
            similarities.add(line)
    return similarities or ""


def sentences(a, b):
    """Return sentences in both a and b"""
    similarities = set()
    a_sent = sent_tokenize(a)
    for sent in a_sent:
        if sent in b:
            similarities.add(sent)
    return similarities or ""


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    similarities = set()
    a_sub = get_substrings(a, n)
    for sub in a_sub:
        if sub in b:
            similarities.add(sub)
    return similarities or ""


def get_substrings(string, n):
    substrings = []
    for i in range(len(string) - n + 1):
        substrings.append(string[i:n+i])
    return substrings
