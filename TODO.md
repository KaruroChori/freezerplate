## v0.2.1

- [x] Make `<?` and `?>` user configurable (via XML file)
- [x] Introduce "custom steps" as part of the XML used to generate the parser instead of requiring manual work.
- [x] Flag to force override during generation
- [x] Initial support for escaping in some classes of languages
- [x] Generate index of files (so to call them with a decent and stable name)
- [x] Added support for raw files not needing templating
- [x] Permissions from the original file are not preserved. Well, do it.
- [x] Macro to simplify access to the XML document
- [x] Add man page

## v0.2.3

- [ ] Fill in the man page for generated templates
- [ ] Complete the logic in the `LICENCE.md` template
- [ ] Document macros and helpers in the main documentation

## Later (in order of increasing importance)

- [ ] ~~Accept skippable files/directories/file types (via XML file)~~ maybe leave it as part of the custom code instead.
- [ ] Add opts to make fully static builds
- [ ] WASM/WASI builds
- [ ] Decompression might be performed more than once at the moment, since past deflated fragments are not stored.
- [ ] File/Line referred to the original source files to more easily navigate compilation errors?
- [ ] Only LF is being handled properly. CRLF ending style is not.
