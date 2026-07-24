# Question 1 - Duplicate Detection, Backup & Reporting Script

## Commands and Explanations

**chmod +x process_submissions.sh**
Grants execute permission to the script so it can be run directly with ./, 
since Linux requires explicit execute permission on shell scripts before 
they can be executed.

**ls submission**
Lists the test submission files (a.txt, b.txt, c.txt) to confirm the 
input directory is set up correctly before running the script.

**./process_submissions.sh ./submission**
Executes the script against the submission folder. The script computes an 
MD5 checksum for each file to detect duplicate content (not just duplicate 
filenames), copies only unique files to a timestamped backup folder, and 
logs all errors separately using stderr redirection (2>>).

**cat report.txt**
Displays the generated report showing 3 files processed, 1 duplicate 
detected (b.txt had identical content to a.txt), and 2 files successfully 
backed up. This confirms the deduplication logic worked correctly.

**cat errors.log**
Shows the error log is empty, confirming no file access, permission, or 
copy errors occurred during processing — demonstrating that error output 
was successfully separated from normal output using stderr redirection.

**ls backup_unique_***
Verifies the backup folder contains only a.txt and c.txt, and excludes 
b.txt, proving that the MD5-checksum-based duplicate detection correctly 
identified and skipped the duplicate file during backup.

## Key Techniques Used
- md5sum: generates content-based hashes to detect true duplicates 
  regardless of filename
- grep -q: silently checks if a hash already exists in the tracking file
- 2>> (stderr append redirection): separates error messages from normal 
  output into errors.log
- >> vs >: append (>>) used for logs/reports that build up over the loop; 
  truncate (>) used once at the start to reset files
- mkdir -p: creates the backup directory without failing if it already exists