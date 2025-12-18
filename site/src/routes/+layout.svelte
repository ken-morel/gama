<script>
    import "@svelteness/kit-docs/client/polyfills/index.js";
    import "@svelteness/kit-docs/client/styles/normalize.css";
    import "@svelteness/kit-docs/client/styles/fonts.css";
    import "@svelteness/kit-docs/client/styles/theme.css";
    import "@svelteness/kit-docs/client/styles/vars.css";

    import { page } from "$app/stores";

    import {
        createKitDocsLoader,
        KitDocs,
        KitDocsLayout,
        Button,
        SocialLink,
        createSidebarContext,
    } from "@svelteness/kit-docs";

    export let data;

    let { meta, sidebar } = data;
    $: ({ meta, sidebar } = data);

    const navbar = {
        links: [
            { title: "Docs", slug: "/docs", match: /\/docs/ },
            {
                title: "Tutorials",
                slug: "/docs/tutorials",
                match: /\/docs\/tutorials/,
            },
        ],
    };

    const { activeCategory } = createSidebarContext(sidebar);

    $: category = $activeCategory ? `${$activeCategory}: ` : "";
    $: title = meta ? `${category}${meta.title} | Gama` : null;
    $: description = meta?.description;
</script>

<svelte:head>
    {#key $page.url.pathname}
        {#if title}
            <title>{title}</title>
        {/if}
        {#if description}
            <meta name="description" content={description} />
        {/if}
    {/key}
</svelte:head>

<KitDocs {meta}>
    <KitDocsLayout {navbar} {sidebar}>
        <div slot="navbar-left">
            <div class="logo">
                <Button href="/">
                    <img
                        width="100"
                        src="/gama-text.png"
                        alt="gama logo with text"
                    />
                </Button>
            </div>
        </div>

        <div class="socials" slot="navbar-right-alt">
            <SocialLink
                type="gitHub"
                href="https://github.com/ken-morel/gama"
            />
        </div>

        <slot />
    </KitDocsLayout>
</KitDocs>

<style>
    .logo :global(a) {
        color: rgb(var(--kd-color-inverse));
        display: flex;
        align-items: center;
        justify-content: center;
    }

    .logo :global(svg) {
        height: 36px;
        overflow: hidden;
    }

    .socials {
        display: flex;
        margin-left: -0.25rem;
    }
</style>
