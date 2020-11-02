---
to: <%= name %>/<%= name %>.h
---
#ifndef NAHFPA_<%= name.toUpperCase() %>_H
#define NAHFPA_<%= name.toUpperCase() %>_H

typedef struct <%= name %> <%= name %>;

<%= name %>* <%= name %>_new();
void <%= name %>_free(<%= name %>* self);

#endif //NAHFPA_<%= name.toUpperCase() %>_H
