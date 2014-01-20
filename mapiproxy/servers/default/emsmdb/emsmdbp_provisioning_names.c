#include "dcesrv_exchange_emsmdb.h"
#include <libmapi/libmapi.h>
#include <string.h>


struct locale_names {
	const char *locale;
	const char *names[17];
};

static const struct locale_names folders_names[] =
{
	{"en", {NULL, "Root", "Deferred Action", "Spooler Queue", "Common Views", "Schedule", "Finder", "Views", "Shortcuts", "Reminders", "To-Do", "Tracked Mail Processing", "Top of Information Store", "Inbox", "Outbox", "Sent Items", "Deleted Items"}},
	{"es", {NULL, "Raíz", "Deferred Action", "Spooler Queue", "Common Views", "Programación", "Buscador", "Vistas", "Accesos directos", "Recordatorios", "To-Do", "Tracked Mail Processing", "Top of Information Store", "Carpeta de entrada", "Carpeta de salida", "Elementos enviados", "Elementos Borrados"}},
	{NULL}
};

struct locale_special_names {
	const char *locale;
	const char *names[SPECIAL_FOLDERS_SIZE];
};

static const struct locale_special_names special_folders[] =
{
	{"en", {"Drafts", "Calendar", "Contacts", "Tasks", "Notes", "Journal"}},
	{"es", {"Borradores", "Calendario", "Contactos", "Tareas", "Notas", "Diario"}},
	{NULL}
};

const char **emsmdbp_get_folders_names(struct emsmdbp_context *emsmdbp_ctx)
{
	uint32_t i, locale_len;
	const char *locale = mapi_get_locale_from_lcid(emsmdbp_ctx->userLanguage);
	if (locale == NULL) {
		return (const char **)folders_names[0].names;
	}
	locale_len = strlen(locale);
	for (i = 0; folders_names[i].locale; i++) {
		if (strlen(folders_names[i].locale) == locale_len &&
		    strncmp(locale, folders_names[i].locale, locale_len) == 0) {
			return (const char **)folders_names[i].names;
		}
	}
	if (locale_len > 2 && locale[2] == '_') {
		// Search only for the first chars of the locale.
		// e.g. en_UK will match against en_US
		for (i = 0; folders_names[i].locale; i++) {
			if (strncmp(locale, folders_names[i].locale, 2) == 0) {
				return (const char **)folders_names[i].names;
			}
		}
	}
	return (const char **)folders_names[0].names;
}

const char **emsmdbp_get_special_folders(struct emsmdbp_context *emsmdbp_ctx)
{
	uint32_t i, locale_len;
	const char *locale = mapi_get_locale_from_lcid(emsmdbp_ctx->userLanguage);
	if (locale == NULL) {
		return (const char **)special_folders[0].names;
	}
	locale_len = strlen(locale);
	for (i = 0; special_folders[i].locale; i++) {
		if (strlen(special_folders[i].locale) == locale_len &&
		    strncmp(locale, special_folders[i].locale, locale_len) == 0) {
			return (const char **)special_folders[i].names;
		}
	}
	if (locale_len > 2 && locale[2] == '_') {
		// Search only for the first chars of the locale.
		// e.g. en_UK will match against en_US
		for (i = 0; special_folders[i].locale; i++) {
			if (strncmp(locale, special_folders[i].locale, 2) == 0) {
				return (const char **)special_folders[i].names;
			}
		}
	}
	return (const char **)special_folders[0].names;
}
