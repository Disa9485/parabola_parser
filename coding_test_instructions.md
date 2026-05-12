# Junior Software Engineer — Coding Test

## Overview
You are given a text file, `parabola_data.txt`, containing 1,200 data points that trace a parabolic curve. The file is CSV-formatted with a header row (`x,y`) followed by comma-separated x,y values. The data contains a small amount of random noise to reflect realistic measurement conditions.

You may use any programming language and any libraries you prefer, but keep in mind that we are a C#/Rust/C++ shop. Please submit your source code along with a short README explaining how to run it and describing any design decisions or tradeoffs you made.

There are libraries that can trivialize some of these tasks, and the entire thing could be trivialized through the use of LLMs. Please do not trivialize the task(s), as the goal is to provide a basis for discussion. The code you produce will be used as a foundation for the technical portion of your interview. 

## Ground Rules
- Complete as many tasks as you can. We do not expect you to finish all of them. Complete the core tasks, and at least two intermediate tasks. If your schedule and motivation allows, complete more. 
- Prioritize clean, readable code over clever code.
- Document what you do.
- Handle edge cases and malformed input where reasonable.
- Include at least a few tests if time permits.
- Complete this task at least 48 hours before your interview. 

---

## Core Tasks (Expected)

**1. Parse the file.** Read `parabola_data.txt` and load the 1,200 points into an appropriate data structure. Your parser should skip the header row and handle whitespace gracefully.

**2. Basic statistics.** Report the following for both x and y: count, min, max, mean, median, and standard deviation.

**3. Find the vertex.** Identify the approximate vertex (minimum or maximum) of the parabola from the data. Report its x and y coordinates.

**4. Validate the data.** Detect and report any malformed rows, missing values, or obvious outliers. Describe your definition of "outlier."

---

## Intermediate Tasks (Preferred)

**5. Curve fitting.** Fit the equation `y = a(x − h)² + k` to the data. Report the values of `a`, `h`, and `k`, and the residual error of your fit. You may use a library (e.g., NumPy's `polyfit`) or implement least-squares manually — tell us which you chose and why.

**6. Plot the data.** Render a scatter plot of the raw points with your fitted curve overlaid. Save it as an image file that can be rendered by commonly available software. 

**7. Split the data.** Separate the points into two halves at the vertex (left branch and right branch). Verify numerically that the two branches are approximately mirror images of each other.

**8. Command-line interface.** Wrap your program so it can be run as `./your_program parabola_data.txt` and optionally accept flags like `--stats`, `--fit`, or `--plot`.

---

## Advanced Tasks (Stretch)

**9. Resampling.** Given an arbitrary x value (possibly between two data points), return an interpolated y value. Handle x values outside the data range with a clear error or extrapolation strategy.

**10. Noise estimation.** Estimate the standard deviation of the noise in the data by comparing each point to your fitted curve.

**11. Performance.** Time your full pipeline (parse → stats → fit → plot). Then describe what would need to change if the file had 12 million points instead of 1,200.

**12. Tests.** Write unit tests covering your parser, your statistics functions, and your curve-fit function. Include at least one test using a synthetic dataset where the correct answer is known.

---

## What We're Looking For
- **Correctness** — does it produce the right answers?
- **Code quality** — is it readable, organized, and reasonably idiomatic for your chosen language?
- **Judgment** — did you pick reasonable libraries, data structures, and abstractions?
- **Communication** — does your README explain what you did and why?

Good luck — we're excited to see what you build.
