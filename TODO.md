- GUI: add circuit list and allow adding circuits as components (partly done)
- GUI: execute tick on first click!
- GUI/DSPatch: forward between settings of circuits components and components settings!
- OPENLF: actually handle config hierarchy
- OPENLF: DspParameter - store origin (type?) so a _process can only set the settings it has itself modified
- CLIF: remove temporary files for memory_link!
- GUI: fix memory leaks!
- GUI: fix circuit display glitches (list circuits, re-open/show already opened, unnamed entries)
- GUI: connectouttoin may remove other connections (only single input allowed!), sync this!
- CLIF: CLIF_LINK_LIBRARIES contains absolute paths! (e.g. hdf5)
- GUI: fix component deletion (segfaults in conn. delete (circuit already deleted?))
- OPENLF: operators macro: we get problems if output dim != input dim (on output[].create...)
- CLIF: keep roughly track of large data allocations (datastores) and flush to disk on some threshold
- CLIF: reimplement disk cache
- CLIF: clifview - more generic datastore display (make it a widget?) (partly done)

Minor:
- COMP_Epi somhow directly store epi processing results in target FlexMav, avoiding one copy!
- GUI: popOut , popOut
- fitler/save only GML files
- use DspParaeter path/filepath (input/output)
  - maybe add dspparameter flags?

Build:
- autouic not correctly called for at least ubuntu 14.04
    uic lfcamconfigurator_linux.ui > ui_lfcamconfigurator_linux.h
    uic toolbox_ui.ui > ui_toolbox_ui.h

Done:
- DSPatch: save/load circuit positions (22.,23.10.15)
- GUI: name load actually with load (23.10.15)
- First load then save button (23.10.15)
- GUI: use circuit coordinates when creating gui and save them when moving components (22.,23.10.15)
- CLIF: on linking memory-only datastore will be lost (23.10.15)
- GUI: delete components and connections in gui! (24.10.15)
- OpenLF/DSPatch use compiled plugin structure (no headers) (25/26.10.15)
- CLIF: TEMP file handling under windows (26.10.15)
- CLIF: clifview: repair and get running on windows (26.10.15)
- build shared libs on linux (27.10.15)
- cannot load any more object with static TLS (ARGHH) (27.10.15 - fixed by changing Linux build to dlls)
- GUI: show error messages for components! (27.10.15)
- GUI: add circuit list and allow adding circuits as components (circuits loaded at startup show already up, 17.10.15)
- CLIF: datastore::link uses file name, not file path (which is unavailable for hdf5?!?) - move to ClifFile (28.10.15)
- CLIF: rewrite core to provide transparent multidimensional data handling throught clif::Datastore (28/29.19.15)
- CLIF: bin/clif: combine clif and ini import (atm clif attributes are lost?) 30.11.15
- fix windows build (30.11.15-2.11.15)
- CLIF: clifview - more generic datastore display (make it a widget?) (partly done 3.11.15)
- CLIF: clifview - windows - automatically show/select on dataset click! (fixed different - 3.11.15)