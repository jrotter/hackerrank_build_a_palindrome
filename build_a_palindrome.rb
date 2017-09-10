class String

  # Is the String object a palindrome?
  def palindrome?
    self == self.reverse
  end

  def palindrome?
    a = 0
    b = length-1
    while b > a
      return false if self[a] != self[b]
      a += 1
      b -= 1
    end
    true
  end

  # Return an array of all indexes where the substr was found within the string
  def indexes(substr)
    outarray = nil
    if (i = self.index(substr))
      outarray = []
      while i
        outarray << i
        i = self.index(substr,i+1)
      end
    end
    outarray
  end

end


module Best

  def Best.init
    @@value = nil
    @@length = 0
  end

  def Best.check(test_string)
    if test_string.length > @@length ||
      ((test_string.length == @@length) && (test_string < @@value))
      @@value = test_string
      @@length = test_string.length
    end
  end

  def Best.length
    @@length
  end

  def Best.value
    @@value
  end

end


# Find all trivial palindromic substrings in a string
# Returns an array of their leftmost index in the string
def find_palindromes_left(s)
  outarray = []
  for x in (0..(s.length-1))
    outarray[x] = 1
  end
  for x in (0..(s.length-1))
    if matches = s.indexes(s[x])
      matches.reverse.each do |y|
        test_string = s[x..y]
        #puts "x=#{x}, y=#{y}, substring = \"#{test_string}\""
        if test_string.palindrome?
          #puts "Found palindrome: \"#{test_string}\""
          outarray[x] = test_string.length
          # Note that I will always find a palindrome at s[x..x] so y won't pass x
          break
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
    outarray[x] = 1
  end
  for y in (s.length-1).downto(0)
    if matches = s.indexes(s[y])
      matches.each do |x|
        test_string = s[x..y]
        #puts "x=#{x}, y=#{y}, substring = \"#{test_string}\""
        if test_string.palindrome?
          #puts "Found palindrome: \"#{test_string}\""
          outarray[y] = test_string.length
          # Note that I will always find a palindrome at s[y..y] so x won't pass y
          break
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
  Best::init()

  # Find all palindromes in s1 (left-indexed) and s2 (right-indexed)
  s1_palindromes = find_palindromes_left(s1)
  #puts s1_palindromes.inspect
  s2_palindromes = find_palindromes_right(s2)
  #puts s2_palindromes.inspect

  s1_substring = nil
  s2_substring = nil
  indexlists = {}

  # Run through all possible substrings of s1
  for x in (0..(s1.length-1))

    # On the off chance that our best palindrome is already bigger than 
    # anything else we'll see from here on out, break
    if (Best::length > ((s1.length-x) + s2.length))
      break
    end

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

        total_bookend_length = left_bookend.length * 2

        # By definition, the bookends are mirror images
        right_bookend = left_bookend.reverse

        # Try the body from s1
        unless y == s1.length-1
          # Only bother if this could be better than the best
          if (total_bookend_length + s1_palindromes[y+1]) >= Best::length
            body = s1[(y+1)..(y+s1_palindromes[y+1])]
            #puts "  Body from s1: \"#{body}\""
            teststring = left_bookend + body + right_bookend
            #puts "    Test string: \"#{teststring}\""
            Best::check(teststring)
          end
        end

        # Reduce indexlist to only palindrome sizes that may 
        # exceed the best we've seen
        reducedlist = []
        max = Best::length - total_bookend_length
        if max == 0 && indexlist[0] == 0
          reducedlist << 0
          start = 1
        else
          start = 0
        end
        for i in start.upto(indexlist.length - 1)
          if s2_palindromes[indexlist[i]-1] > max
            reducedlist = [ indexlist[i] ]
          elsif s2_palindromes[indexlist[i]-1] == max
            reducedlist << indexlist[i]
          end
        end
        puts "Reduced list: #{reducedlist.inspect}"

        # Now loop through the remaining right bookends
        reducedlist.each do |a|
          #b = a + left_bookend.length - 1
          # The right bookend is s2[a..b]
          #puts "  Right bookend: \"#{right_bookend}\" (#{a})"

          if a == 0 
            if y == s1.length-1
              # Try the bookends without a body
              #puts "  No body"
              teststring = left_bookend + right_bookend
              #puts "    Test string: \"#{teststring}\""
              Best::check(teststring)
            end
          else
            # Try the body from s2

            # Only bother if this could be better than the best
            if (total_bookend_length + s2_palindromes[a-1]) >= Best::length
              body = s2[(a-s2_palindromes[a-1])..(a-1)]
              #puts "  Body from s2: \"#{body}\""
              teststring = left_bookend + body + right_bookend
              #puts "    Test string: \"#{teststring}\""
              Best::check(teststring)
            else
              #puts "  Not worth trying palindrome from s2"
            end
          end

        end
      else
        # If a right bookend was not found for x at a given length
        # nothing will be found with a greater length
        break
      end
    end
  end

  if Best::value
    puts Best::value
  else
    puts "-1"
  end

end
