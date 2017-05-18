# Aegir HTTPS

This module enables HTTPS support for sites within the [Aegir Hosting System](http://www.aegirproject.org/) using certificate management services such as [Let's Encrypt](https://letsencrypt.org/), whose support is included.

It provides a cleaner, more sustainable and more extensible implementation that what's currently offered in Aegir SSL within Aegir core, and doesn't require workarounds such as [hosting_le](https://github.com/omega8cc/hosting_le).

## Requirements

1. Aegir 3.9+ or the patch from [Remove 'node_access' check from default hosting_get_servers() calls](https://www.drupal.org/node/2824329#comment-11772591).  See [hosting_certificate_prevent_orphaned_services() causing recursive/loop cache rebuild](https://gitlab.com/aegir/hosting_https/issues/7) for details.
2. If you're running the Nginx Web server and would like to use Let's Encrypt certificates, be sure to prevent Nginx's default configuration from running.  Otherwise, it will prevent this server configuration from allowing access to the challenge directory.
    * `sudo rm /etc/nginx/sites-enabled/default`

## Installation

1. Cleanup old SSL usage.
    * Check that the hostmaster site is not set to Encryption: Required. (e.g. on /hosting/c/hostmaster) to avoid locking yourself out.
    * Edit the server nodes(e.g. /hosting/c/server_master) to not use an SSL service.
    * Disable any of the SSL modules (including hosting_le) you may have already enabled.
2. Switch to the directory where you wish to install the module.
    * cd /var/aegir/hostmaster-7.x-3.x/sites/aegir.example.com/modules/contrib
3. Download this module and the [Dehydrated](https://github.com/lukas2511/dehydrated) library:
    * Option 1: Clone with Git. This command will download the latest release (at the time of this writing). Browse [all releases](https://gitlab.com/aegir/hosting_https/tags).
        * `git clone --recursive --branch 7.x-3.x-alpha4 https://gitlab.com/aegir/hosting_https.git`
    * Option 2: Install with Drush make. (See Below)
4. Surf to Administration » Hosting » Experimental » Aegir HTTPS.
5. Enable at least one certificate service (e.g. Let's Encrypt or Self-signed).
6. Enable at least one Web serrver service (e.g. Apache HTTPS or Nginx HTTPS).
7. Save the configuration.

## Adding Aegir HTTPS with Drush Make

You can use the following makefile to add Hosting HTTPS and the Dehydrated library to your hostmaster site:

```
; hosting_https.make
; Download hosting_https via git until we have a release on Drupal.org.
projects[hosting_https][type] = module
projects[hosting_https][download][type] = git
projects[hosting_https][download][url] = https://gitlab.com/aegir/hosting_https.git
projects[hosting_https][download][branch] = master
projects[hosting_https][subdir] = "aegir"

; Dehydrated for LetsEncrypt.org
libraries[dehydrated][download][type] = git
libraries[dehydrated][download][url] = https://github.com/lukas2511/dehydrated
libraries[dehydrated][destination] = modules/aegir/hosting_https/submodules/letsencrypt/drush/bin
```

To install into an existing site: create a hosting_https.make file, put it in the root of your hostmaster, ie /var/aegir/hostmaster-7.x-3.x, and run the following command:

```
drush make --no-core hosting_https.make --contrib-destination sites/myaegir.com
```

Make sure to set `--contrib-destination`, to put the modules in `sites/myaegir.com`, so you can be sure they are kept when you upgrade Hostmaster.

## Server Set-Up

1. Surf to the Servers tab.
2. Click on the Web server where you'd like HTTPS enabled.
3. Click on the Edit tab.
4. Under Certificate, choose your desired certificate service (and set any of its additional configuration).
5. Under Web, choose the HTTPS option for your Web server (and set any of its additional configuration).
6. Hit the Save button.

## Site Set-Up

1. Ensure that there's a DNS entry for the site that you'd like HTTPS enabled (unless already handled by a wildcard entry pointing to your Aegir server).
2. Verify the site if this hasn't been done since the server was set up with the above steps.  This ensures that the site can respond to the certificate authority's challenge.
3. Edit the site.
4. In the HTTPS Settings section, choose either Enabled or Required.
5. Save the form.
6. Repeat these steps for any other sites for which you'd like to enable HTTPS.

## Certificate Renewals

For the Let's Encrypt certificate service, this should get done automatically via the Let's Encrypt queue. It will run a Verify task on each site every week as site verification is where certificates get renewed if needed. The seven-day default was chosen to match the CA's [rate limits](https://letsencrypt.org/docs/rate-limits/).

## Known Issues

See [the issue queue](https://gitlab.com/aegir/hosting_https/issues).

## Troubleshooting

If you notice that the certificate generation fails you can check the Aegir 'Verify' task logs for details.

### Test the challenge directory

Create a file e.g. called `index.html` in `/var/aegir/config/letsencrypt.d/well-known/acme-challenge/` and test if you can access it over http via http://www.example.com/.well-known/acme-challenge/index.html

If your request is redirected to a *https* url then that could pose a problem when the certificate there is either invalid or expired. Try to remove the redirects.
