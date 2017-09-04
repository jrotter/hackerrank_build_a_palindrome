class String

  # Is the String object a palindrome?
  def palindrome?
    self == self.reverse
  end

  # Return an array of all indexes where the substr was found within the string
  def indexes(substr)
    outarray = nil
    if (i = self.index(substr))
      outarray = []
      z = self.length
      sl = substr.length
      while i
        outarray << i
        if (j = self[(i+1)...z].index(substr))
          i += (j+1)
        else
          i = nil
        end
      end
    end
    outarray
  end

end

# Return the NEW best palindrome
def new_best(test_string,best)
  if best == nil  || 
     (test_string.length > best.length) ||
     ((test_string.length == best.length) && (test_string < best))
    test_string
  else
    best
  end
end

# Find all trivial palindromic substrings in a string
# Returns an array of their leftmost index in the string
def find_palindromes_left(s)
  outarray = []
  for x in (0..(s.length-1))
    outarray[x] = 0
  end
  for x in (0..(s.length-1))
    for y in ((x)..(s.length-1))
      s_substring = s[x..y]
      if s_substring.palindrome?
        if s_substring.length > outarray[x]
          outarray[x] = s_substring.length
        end
      end
    end
  end
  outarray
end

# Find all trivial palindromic substrings in a string
# Returns an array of their rightmost index in the string
def find_palindromes_right(s)
  outarray = []
  for x in (0..(s.length-1))
    outarray[x] = 0
  end
  for x in (0..(s.length-1))
    for y in ((x)..(s.length-1))
      s_substring = s[x..y]
      if s_substring.palindrome?
        if s_substring.length > outarray[y]
          outarray[y] = s_substring.length
        end
      end
    end
  end
  outarray
end

# Read in the data
count = gets.strip.to_i
count.times do |i|
  s1 = gets.strip
  s2 = gets.strip

  # Find all palindromes in s1 (left-indexed) and s2 (right-indexed)
  s1_palindromes = find_palindromes_left(s1)
  #puts s1_palindromes.inspect
  s2_palindromes = find_palindromes_right(s2)
  #puts s2_palindromes.inspect

  best = nil
  s1_substring = nil
  s2_substring = nil
  indexlists = {}

  # Run through all possible substrings of s1
  for x in (0..(s1.length-1))
    for y in (x..(s1.length-1))
      # The left bookend is s1[x..y]
      left_bookend = s1[x..y]
      #puts "Left bookend: \"#{left_bookend}\" (#{x})"

      # This substring is the left bookend of the palindrome.
      # Find all possible right bookends
      indexlists[left_bookend] ||= s2.indexes(left_bookend.reverse)
      indexlist = indexlists[left_bookend]

      # If any right bookends were found...
      if indexlist

        # Loop through the right bookends
        indexlist.each do |a|
          b = a + left_bookend.length - 1
          # The right bookend is s2[a..b]
          right_bookend = s2[a..b]
          #puts "  Right bookend: \"#{right_bookend}\" (#{a})"

          # Try the body from s1
          unless y == s1.length-1
            body = s1[(y+1)..(y+s1_palindromes[y+1])]
            #puts "  Body from s1: \"#{body}\""
            teststring = left_bookend + body + right_bookend
            #puts "    Test string: \"#{teststring}\""
            best = new_best(teststring,best)
          end

          # Try the body from s2
          unless a == 0
            body = s2[(a-s2_palindromes[a-1])..(a-1)]
            #puts "  Body from s2: \"#{body}\""
            teststring = left_bookend + body + right_bookend
            #puts "    Test string: \"#{teststring}\""
            best = new_best(teststring,best)
          end
        end
      else
        # If a right bookend was not found for x at a given length
        # nothing will be found with a greater length
        break
      end
    end
  end

  if best
    puts best
  else
    puts "-1"
  end

end
