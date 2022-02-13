#pragma once

/*! \mainpage WYAG - Write Yourself a Git (in C++)
 *
 * This is my implementation of the great guide WYAG
 * (https://wyag.thb.lt/).
 */

namespace libwyag {

/**
 * Entry function to this library.
 *
 * @param argc Cli argc.
 * @param argv Cli argv.
 * @return Indicates success/failure of command.
 */
int entry(const int argc, const char *argv[]);

}  // namespace libwyag
