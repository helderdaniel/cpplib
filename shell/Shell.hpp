/**
 * Shell utilities
 * hdaniel@ualg.pt may 2020
 */

#ifndef MOOSHAKTOOLS_SHELL_H
#define MOOSHAKTOOLS_SHELL_H

namespace had {

	class Shell {

	public:

		/**
		 * Executes cmd on a shell and returns stdout.
		 * If willing to return also stderr add to the end
		 * of cmd:  2>&1
		 * However this will be mixed
		 *
		 * based on:
		 *	 //https://stackoverflow.com/questions/52164723/how-to-execute-a-command-and-get-return-code-stdout-and-stderr-of-command-in-c
		 *
		 * @param cmd		string with the command line to execute
		 * @param output    stdout from the execution of cmd (previous contents are cleared)
		 * @return          execution code of cmd
		 */
		static int execute(std::string cmd, std::string &output) {
			const int bufsize = 128;
			std::array<char, bufsize> buffer;
			output = ""; //clear string

			auto pipe = popen(cmd.c_str(), "r");
			if (!pipe) throw std::runtime_error("popen() error: " + std::to_string(errno));

			size_t count;
			do {
				if ((count = fread(buffer.data(), 1, bufsize, pipe)) > 0) {
					output.insert(output.end(), std::begin(buffer), std::next(std::begin(buffer), count));
				}
			} while (count > 0);

			return pclose(pipe);
		}
	};

}

#endif //MOOSHAKTOOLS_SHELL_H
