# This test is essential :)
def is_palindrome?(a)
  a == a.reverse
end

# Find the minimal string that's needed to "palindrome" this string to the right
def search_string_right(s)
  best = nil
  for i in (s.length).downto(1)
    unless s.index(s[(i-1)..(s.length-1)].reverse)
      return s[0...i].reverse
    end  
  end
  if s.length == 1
    s
  else
    s[0..(s.length-2)].reverse
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

# Read in the data
count = gets.strip.to_i
count.times do |i|
  s1 = gets.strip
  s2 = gets.strip

# Find all non-trivial palindromes in s2 by brute force
s2_palindromes = {}
for x in (0..(s2.length-1))
  for y in ((x+1)..(s2.length-1))
    s2_substring = s2[x..y]
    if is_palindrome?(s2_substring)
      unless s2_palindromes[y] && s2_palindromes[y] >= s2_substring.length
        s2_palindromes[y] = s2_substring.length
      end
    end
  end
end
puts s2_palindromes.inspect

  best = nil
  s1_substring = nil
  s2_substring = nil

  # Run through all possible substrings of s1
  for x in (0..(s1.length-1))
    if s2.index(s1[x])  # If the first character of s1 is not in s2, forget it
      for y in (x..(s1.length-1))
        # s1[x..y] is our current substring of s1
        s1_substring = s1[x..y]

        # If we can't find a match for the first N characters of s1,
        # we won't be able to find a match for the first N+1 characters of s1
        break unless s2.index(search_string_right(s1_substring))

        # Now look at all substrings of s2
        for b in (s2.length-1).downto(0)
          for a in b.downto(0)

            # s2[a..b] is our current substring of s2
            s2_substring = s2[a..b]
            test_string = s1_substring + s2_substring
            #puts "== #{s1_substring} #{s2_substring}"

            if is_palindrome?(test_string)
              #puts "PALINDROME FOUND: #{test_string}"
              best = new_best(test_string,best)

              # In the case where we have equal parts from s1 and s2 (and there's 
              # more of s2 left), we can now use the s2_palindromes hash to derive 
              # the rest and then break
              if ((s2_substring.length == s1_substring.length) && (a != 0))
                #puts "Lengths match"
                #puts "b == #{b}"
                #puts "s1_substring.length == #{s1_substring.length}"
                center_end = b-s1_substring.length
                if s2_palindromes[center_end]
                  center_begin = center_end-s2_palindromes[center_end]+1
                  center_palindrome = s2[(center_begin)..(center_end)]
                  #puts "Palindrome found in s2 of length #{s2_palindromes[center_end]}: \"#{center_palindrome}\""
                  full_palindrome = s1_substring + center_palindrome + s2_substring
                  #puts "Full Palindrome: \"#{full_palindrome}\""
                else
                  center_palindrome = s2[center_end]
                  #puts "Calculated length 1 palindrome \"#{center_palindrome}\""
                  full_palindrome = s1_substring + s2[center_end] + s2_substring
                  #puts "Adding calculated best: \"#{full_palindrome}\""
                end
                best = new_best(full_palindrome,best)
                break
              end
            else
              if s2_substring.length >= s1_substring.length
                break
              end
            end
          end
        end
      end
    end
  end
  if best
    puts best
  else
    puts "-1"
  end
end

